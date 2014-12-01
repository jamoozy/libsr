#include <stdio.h>
#include <string.h>
#include <values.h>
#include <math.h>
#include <assert.h>

#include "common/geom.h"
#include "line_test.h"

// The line test's context (used by most functions here).
static line_test_context_t context;

// Resets the context for testing.
static void _reset(const paleo_stroke_t* stroke) {
  context.stroke = stroke;
}

// Does the line segment test on the ranges provided.
//   first_i: Index (incl.) of the first point to use.
//   last_i: Index (excl.) of the last point to use.
static inline line_test_result_t* _line_segment_test(int first_i, int last_i);

line_test_result_t* line_test(const paleo_stroke_t* stroke) {
  _reset(stroke);

  return NULL;
}

// Creates the best fit line segment between the two point indexes and stores it
// in the context.
static inline void _best_fit_line_seg(int first_i, int last_i);

// Computes the projection of p to the ideal line.
//   proj: Return value (projection)
//   p: The point to project.
static inline void _projection_to_ideal(point2d_t* proj, const point2d_t* p);

// Computes the orthogonal distance from the point to the ideal line.
static inline double _distance_to_ideal(const point2d_t* p);

// Convenience macro to set up the context's result as having failed.
#define SET_FAIL(msg, args...) { \
  const int room = strlen(msg) + 100; \
  context.result->fmsg = realloc( \
      context.result->fmsg, room * sizeof(char)); \
  if (room <= snprintf(context.result->fmsg, room, msg, args)) { \
    fprintf(stderr, "Wrote too many bytes."); \
    assert(0); \
  } \
  context.result->possible = 0; \
}

// Convenience macro that does the same as SET_FAIL and then returns the result.
#define SET_FAIL_RTN(msg, args...) SET_FAIL(msg, args) return context.result;

static inline line_test_result_t* _line_segment_test(int first_i, int last_i) {
  context.result = calloc(1, sizeof(line_test_result_t));
  context.result->possible = 1;

  _best_fit_line_seg(first_i, last_i);

  double od2 = 0;  // Orthogonal distance squared.
  for (int i = first_i; i < last_i; i++) {
    od2 += _distance_to_ideal(&context.stroke->pts[i].p2d);
  }
  context.result->lse = od2 / context.stroke->px_length;
  if (context.result->lse >= PALEO_THRESH_G) {
    SET_FAIL_RTN("Line LSE too large: %.2f >= %.2f",
        context.result->lse, PALEO_THRESH_G);
  }

  context.result->fa = 0;
  for (int i = first_i + 1; i < last_i; i++) {
    // Compute the projections to the line.
    point2d_t proj_a, proj_b;
    _projection_to_ideal(&proj_a, &context.stroke->pts[i-1].p2d);
    _projection_to_ideal(&proj_b, &context.stroke->pts[i].p2d);

    // Add quad area to feature area (order is important!).
    context.result->fa += geom_quad_area(&proj_b, &proj_a,
        &context.stroke->pts[i-1].p2d, &context.stroke->pts[i].p2d);
  }

  return context.result;
}

static inline void _best_fit_line_seg(int first_i, int last_i) {
  assert(0 <= first_i && first_i < last_i && last_i <= context.stroke->num_pts);

  // Used the algorithm found here:
  //   http://faculty.cs.niu.edu/~hutchins/csci230/best-fit.htm
  long sum_x = 0, sum_y = 0, sum_x2 = 0, sum_xy = 0;
  for (int i = first_i; i < last_i; i++) {
    sum_x += context.stroke->pts[i].x;
    sum_y += context.stroke->pts[i].y;
    sum_x2 += context.stroke->pts[i].x * context.stroke->pts[i].x;
    sum_xy += context.stroke->pts[i].x * context.stroke->pts[i].y;
  }

  const int num = last_i - first_i;
  double x_mean = sum_x / num;
  double y_mean = sum_y / num;

  // The equivalent of the return value -- assigning values to the ideal line.
  double denom = (sum_x2 - sum_x * x_mean);
  if (abs(denom) > 0.0000001) {  // Some number very close to 0.
    context.ideal_line.slope = (sum_xy - sum_x * y_mean) / denom;
    context.ideal_line.y_int = y_mean - context.ideal_line.slope * x_mean;
    context.ideal_line.p0.x = 0;
    context.ideal_line.p0.y = context.ideal_line.y_int;
  } else {  // Avoid div-by-0.
    context.ideal_line.slope = DBL_MAX;
    context.ideal_line.y_int = DBL_MAX;

    // X coordinate of this line should be average of all the points in the
    // line.
    context.ideal_line.p0.x = 0;
    context.ideal_line.p0.y = LONG_MAX;
    for (int i = first_i; i < last_i; i++) {
      context.ideal_line.p0.x += context.stroke->pts[i].x;
    }
    context.ideal_line.p0.x /= num;
  }

  context.ideal_line.theta = atan2(
    context.stroke->pts[last_i].y - context.stroke->pts[first_i].y,
    context.stroke->pts[last_i].x - context.stroke->pts[first_i].x);
}

static inline double _distance_to_ideal(const point2d_t* p) {
  point2d_t proj;   // projection return value.
  _projection_to_ideal(&proj, p);
  return point2d_distance(&proj, p);
}

static inline void _projection_to_ideal(point2d_t* proj, const point2d_t* p) {
  assert(context.ideal_line.slope != 0 || context.ideal_line.y_int != 0);

  // Avoid div-by-0.
  if (context.ideal_line.slope >= DBL_MAX) {
    proj->x = context.ideal_line.p0.x;
    proj->y = p->y;
  } else {
    const double m_x = 1;
    const double m_y = context.ideal_line.slope;

    // This is the dividend of two dot products:
    //    v . s / s . s
    // where v is the vector from p0 to p, and u is the vector defined by the
    // slope: i.e., <1,m>
#define S (context.ideal_line.slope)
#define P (context.ideal_line.p0)
    double dist = ((p->x - P.x) + (p->y - P.y) * S) / (1 + S * S);
    proj->x = P.x + dist;
    proj->y = P.y + dist * S;
#undef P
#undef S
  }
}

void line_test_deinit() {

}
