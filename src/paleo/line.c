/*! \file line.c
 * Implements the interface in line.h
 *
 * \addtogroup pal
 * \{
 */


#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <values.h>
#include <math.h>
#include <assert.h>

#include "common/geom.h"

#define result res.res[0]   // Alter test macros to work with unique struct.
#include "test_macros.h"

// Redefine CHECK_RTN_RESULT to work with this unique struct.
#ifdef CHECK_RTN_RESULT
# undef CHECK_RTN_RESULT
#endif

#define CHECK_RTN_RESULT(cond, msg, ...) do { \
  if (!(cond)) { \
    SET_FAIL(msg, ##__VA_ARGS__); \
    return &context.res; \
  } \
} while (0)

#include "line.h"


////////////////////////////////////////////////////////////////////////////////
// ---------------------------- Shape Functions ----------------------------- //
////////////////////////////////////////////////////////////////////////////////

pal_line_t* pal_line_create() {
  pal_line_t* self = calloc(1, sizeof(pal_line_t));
  self->pts = calloc(self->num = 2, sizeof(point2d_t*));
  return self;
}

pal_line_t* pal_line_create_points_with_points(
    const point2d_t* a, const point2d_t* b) {
  pal_line_t* self = pal_line_create();
  memcpy(&(self->pts[0]), a, sizeof(point2d_t));
  memcpy(&(self->pts[1]), b, sizeof(point2d_t));
  return self;
}

pal_line_t* pal_line_create_points_with_longs(
    long ax, long ay, long bx, long by) {
  pal_line_t* self = pal_line_create();
  self->pts[0].x = ax;
  self->pts[0].y = ay;
  self->pts[1].x = bx;
  self->pts[1].y = by;
  return self;
}



////////////////////////////////////////////////////////////////////////////////
// ----------------------------- Test Functions ----------------------------- //
////////////////////////////////////////////////////////////////////////////////

//! The line test's context (used by most functions here).
static pal_line_context_t context;

void pal_line_init() { bzero(&context, sizeof(pal_line_context_t)); }

void pal_line_deinit() {
  for (int i = 0; i < context.res.num; i++) {
    free(context.res.res[i].line.pts);
  }
  free(context.res.res);
}

/*! Resets the line context.
 *
 * \param stroke The stroke to recognize.
 * \param num The number of joints in the line.
 */
static inline void _reset(const pal_stroke_t* stroke, int num) {
  pal_line_deinit();    // Free memory.
  pal_line_init();      // Reset object.

  context.stroke = stroke;
  context.res.num = num;
  context.res.res = calloc(num, sizeof(pal_line_sub_result_t));
  for (int i = 0; i < num; i++) {
    context.res.res[i].possible = 1;
  }
}



/*! Does the line segment test on the ranges provided.
 *
 * \param first_i Index (incl.) of the first point to use.
 * \param last_i Index (excl.) of the last point to use.
 */
static inline void _line_test(int first_i, int last_i);

const pal_line_result_t* pal_line_test(const pal_stroke_t* stroke) {
  _reset(stroke, 1);

  if (stroke->num_crnrs == 2 || stroke->num_crnrs == 3) {
    _line_test(0, stroke->num_pts);
  }
  return &context.res;
}

const pal_line_result_t* pal_pline_test(const pal_stroke_t* stroke) {
  // Check DCR value.
  CHECK_RTN_RESULT(stroke->dcr >= PAL_THRESH_J,
      "Stroke DCR val too low: %.2f < %.2f", stroke->dcr, PAL_THRESH_J);

  // Init/reset the context.
  _reset(stroke, stroke->num_crnrs);

  // Do the line test for each sub-line.
  double avg_lse = 0;   // also compute average LSE
  for (int i = 1; i < stroke->num_crnrs; i++) {
    _line_test(stroke->crnrs[i-1]->i, stroke->crnrs[i]->i);

    CHECK_RTN_RESULT(context.res.res[0].possible,
        "Does not pass line test in sub-seg %d", i);

    memcpy(&context.res.res[i], &context.res.res[0],
        sizeof(pal_line_sub_result_t));
    avg_lse += context.res.res[0].lse;
  }

  // Compute avg LSE and verify it's okay.
  bzero(&context.res.res[0], sizeof(pal_line_sub_result_t));
  context.res.res[0].lse = avg_lse / stroke->num_crnrs;
  CHECK_RTN_RESULT(context.res.res[0].lse < PAL_THRESH_I,
      "Avg LSE too high: %.2f >= %.2f", context.res.res[0].lse, PAL_THRESH_I);

  // Everything checks out.  Create the line and return it.
  context.res.res[0].possible = 1;
  context.res.res[0].line.num = stroke->num_crnrs;
  context.res.res[0].line.pts = calloc(
      context.res.res[0].line.num, sizeof(point2d_t));
  for (int i = 0; i < stroke->num_crnrs; i++) {
    memcpy(&context.res.res[0].line.pts[i], stroke->crnrs[i],
        sizeof(point2d_t));
  }
  return &context.res;
}

/*! Creates the best fit line segment between the two point indexes and stores it
 * in the context.
 *
 * \param first_i The first 
 */
static inline void _best_fit_line_seg(int first_i, int last_i);

/*! Computes the projection of p to the ideal line.
 *
 * \param proj Return value (projection)
 * \param p The point to project.
 */
static inline void _projection_to_ideal(point2d_t* proj, const point2d_t* p);

/*! Computes the orthogonal distance from the point to the ideal line.
 *
 * \param p The point.
 *
 * \return The distance from \c p to the ideal line in px.
 */
static inline double _distance_to_ideal(const point2d_t* p);

static inline void  _line_test(int first_i, int last_i) {
  // Reset 0th 
  bzero(&context.res.res[0], sizeof(pal_line_sub_result_t));
  context.res.res[0].possible = 1;

  _best_fit_line_seg(first_i, last_i);

  double px_len = 0;  // Length of this sub-stroke.
  double od2 = 0;     // Orthogonal distance squared.
  for (int i = first_i; i < last_i; i++) {
    double d = _distance_to_ideal(&context.stroke->pts[i].p2d);
    od2 += d * d;
    if (i > first_i) {
      px_len += point2d_distance(
          &context.stroke->pts[i-1].p2d, &context.stroke->pts[i].p2d);
    }
  }
  context.res.res[0].lse = od2 / px_len;
  if (context.res.res[0].lse >= PAL_THRESH_G) {
    SET_FAIL_RTN("Line LSE too large: %.2f >= %.2f",
        context.res.res[0].lse, PAL_THRESH_G);
  }

  context.res.res[0].fa = 0;
  for (int i = first_i + 1; i < last_i; i++) {
    // Compute the projections to the line.
    point2d_t proj_a, proj_b;
    _projection_to_ideal(&proj_a, &context.stroke->pts[i-1].p2d);
    _projection_to_ideal(&proj_b, &context.stroke->pts[i].p2d);

    // Add quad area to feature area (order is important!).
    context.res.res[0].fa += geom_quad_area(&proj_b, &proj_a,
        &context.stroke->pts[i-1].p2d, &context.stroke->pts[i].p2d);
  }

  if (context.res.res[0].fa / px_len >= PAL_THRESH_H) {
    SET_FAIL_RTN("FA too large: %.2f / %.2f = %.2f >= %.2f",
        context.res.res[0].fa, px_len, context.res.res[0].fa / px_len,
        PAL_THRESH_H);
  }

  // Everything checks out.  Create the line and return.
  context.res.res[0].line.num = 2;
  context.res.res[0].line.pts = calloc(2, sizeof(point2d_t));
  memcpy(&context.res.res[0].line.pts[0],
      &context.stroke->pts[first_i], sizeof(point2d_t));
  memcpy(&context.res.res[0].line.pts[1],
      &context.stroke->pts[last_i-1], sizeof(point2d_t));
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
    context.ideal.slope = (sum_xy - sum_x * y_mean) / denom;
    context.ideal.y_int = y_mean - context.ideal.slope * x_mean;
    context.ideal.p0.x = 0;
    context.ideal.p0.y = context.ideal.y_int;
  } else {  // Avoid div-by-0.
    context.ideal.slope = DBL_MAX;
    context.ideal.y_int = DBL_MAX;

    // X coordinate of this line should be average of all the points in the
    // line.
    context.ideal.p0.x = 0;
    context.ideal.p0.y = LONG_MAX;
    for (int i = first_i; i < last_i; i++) {
      context.ideal.p0.x += context.stroke->pts[i].x;
    }
    context.ideal.p0.x /= num;
  }

  context.ideal.theta = atan2(
    context.stroke->pts[last_i].y - context.stroke->pts[first_i].y,
    context.stroke->pts[last_i].x - context.stroke->pts[first_i].x);
}

static inline double _distance_to_ideal(const point2d_t* p) {
  point2d_t proj;   // projection return value.
  _projection_to_ideal(&proj, p);
  return point2d_distance(&proj, p);
}

static inline void _projection_to_ideal(point2d_t* proj, const point2d_t* p) {
  assert(context.ideal.slope != 0 || context.ideal.y_int != 0);

  // Avoid div-by-0.
  if (context.ideal.slope >= DBL_MAX) {
    proj->x = context.ideal.p0.x;
    proj->y = p->y;
  } else {
    const double m_x = 1;
    const double m_y = context.ideal.slope;

    // This is the dividend of two dot products:
    //    v . s / s . s
    // where v is the vector from p0 to p, and u is the vector defined by the
    // slope: i.e., <1,m>
#define S (context.ideal.slope)
#define P (context.ideal.p0)
    double dist = ((p->x - P.x) + (p->y - P.y) * S) / (1 + S * S);
    proj->x = P.x + dist;
    proj->y = P.y + dist * S;
#undef P
#undef S
  }
}

/*! \} */
