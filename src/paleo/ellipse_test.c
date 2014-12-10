#include <config.h>
#include <strings.h>
#include <values.h>
#include <math.h>

#include "common/util.h"
#include "common/point.h"
#include "common/geom.h"
#include "test_macros.h"
#include "ellipse_test.h"



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Up & Down -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// The ellipse context for testing.
static ellipse_test_context_t e_context;

void ellipse_test_init() { bzero(&e_context, sizeof(ellipse_test_context_t)); }

void ellipse_test_deinit() { }

static inline void _reset_el(const paleo_stroke_t* stroke) {
  // 0-out e_context.
  bzero(&e_context, sizeof(ellipse_test_result_t));

  // Set stroke and initialize.
  e_context.stroke = stroke;
  e_context.result.possible = 1;
}


// The circle context for testing.
static circle_test_context_t c_context;

void circle_test_init() { bzero(&c_context, sizeof(circle_test_context_t)); }

void circle_test_deinit() { }

static inline void _reset_cir(const paleo_stroke_t* stroke) {
  // 0-out e_context.
  bzero(&c_context, sizeof(ellipse_test_result_t));

  // Set stroke and make this
  c_context.stroke = stroke;
  c_context.result.possible = 1;
}



////////////////////////////////////////////////////////////////////////////////
// --------------------------- The Ellipsis Test ---------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Make macros work for e_context.
#define context e_context

const ellipse_test_result_t* ellipse_test(const paleo_stroke_t* stroke) {
  CHECK_RTN_RESULT(stroke->closed, "Stroke not closed.");

  _reset_el(stroke);

  // Init the ideal ellipse.
  bzero(&e_context.ideal.center, sizeof(point2d_t));
  e_context.ideal.minor.len = DBL_MAX;

  // Compute major and minor axes and center.
  for (int i = 0; i < stroke->num_pts; i++) {
    point2d_accum(&e_context.ideal.center, &stroke->pts[i].p2d);

    // Loop through points 2nd time to compare all point pairs.
    for (int j = 0; j < stroke->num_pts; j++) {
      if (i == j) { continue; }  // Skip comparisons with the same point.

      // Compute len to determine whether this pair could be the major axis.
      double dist = point2d_distance(&stroke->pts[i].p2d, &stroke->pts[j].p2d);
      if (dist > e_context.ideal.major.len) {
        e_context.ideal.major.len = dist;
        e_context.ideal.major.i = i;
        e_context.ideal.major.j = j;
      }
    }

    // Also compute the Yu feature area.
    if (i == 0) { continue; }
    e_context.result.fa += geom_triangle_area(
        &e_context.ideal.center, &stroke->pts[i].p2d, &stroke->pts[i-1].p2d);
  }
  point2d_div(&e_context.ideal.center, stroke->num_pts);

  // Find line orthogonal to maj through center.
  point2d_t orth[2];
  geom_ortho_line(&orth[0], &orth[1],
      &stroke->pts[e_context.ideal.major.i].p2d,
      &stroke->pts[e_context.ideal.major.j].p2d,
      &e_context.ideal.center);

  // Loop through the points again to find an intersection between the stroke
  // and the perpendicular intersection of the major axis through the center.
  short which = 0;   // Keep track of which is being assigned.
  for (int i = 1; i < stroke->num_pts; i++) {
    point2d_t isect;
    if (geom_seg_line_intersection(&isect,
          &stroke->pts[i-1].p2d, &stroke->pts[i].p2d, &orth[0], &orth[1])) {
      memcpy(
          (which++ % 2) ? &e_context.ideal.minor.b : &e_context.ideal.minor.a,
          &isect, sizeof(point2d_t));
    }
  }
  assert(which >= 2);   // should have assigned at least 2 points.

  // Compute minor length.
  e_context.ideal.minor.len = point2d_distance(
      &e_context.ideal.minor.a, &e_context.ideal.minor.b);

  // Check that the NDDE is sufficiently high, or the ellipse is sufficiently
  // small.
  CHECK_RTN_RESULT(stroke->ndde > PALEO_THRESH_K ||
      e_context.ideal.major.len < PALEO_THRESH_L,
          "NDDE (%.2f) too small for maj length (%.2f)",
          stroke->ndde, e_context.ideal.major.len);

  // Check that the feature area error (FA / ideal's area) is sufficiently
  // small.  From the paper:
  //
  //    The feature area error (feature area divided by area of the ideal
  //    ellipse, as described in [12]) must be less than some threshold (M).
  //
  // Yu's paper (which is [12]) computes the feature area by summing all the
  // triangles defined by the stroke's sub-segments and the center point of the
  // ellipse.
  //
  // Paulson's paper also mentions how to deal with overtraced shapes:
  //
  //    If the stroke is overtraced, then it is broken into sub-strokes at each
  //    2π interval in the direction graph. All of the sub-strokes (minus the
  //    last sub-stroke, as it may be an incomplete ellipse) are then fit to
  //    ellipses and the error becomes the average feature area error across
  //    each sub-ellipse.
  //
  // Instead of using this process, the following code just normalizes by the
  // total angle traversed by the stroke about the center.
  context.angle = 0;
  e_context.result.fa = 0;
  for (int i = 1; i < stroke->num_pts; i++) {
    e_context.result.fa += geom_triangle_area(
        &e_context.ideal.center, &stroke->pts[i-1].p2d, &stroke->pts[i].p2d);

    double d_angle =
      point2d_angle_to(&e_context.ideal.center, &stroke->pts[i].p2d) -
      point2d_angle_to(&e_context.ideal.center, &stroke->pts[i-1].p2d);

    NORM_ANGLE(d_angle);
    context.angle += d_angle;
  }
  e_context.result.fa *= abs(context.angle) / (2 * M_PIl);  // Normalization.

  double fae = 4 * e_context.result.fa /
    (M_PIl * e_context.ideal.major.len * e_context.ideal.minor.len);
  CHECK_RTN_RESULT(fae < PALEO_THRESH_M,
      "FAE too large: %.2f >= %.2f", fae, PALEO_THRESH_M);

  // If it all checks out, create the ellipse based on maj/min/center.
  //
  // This is based on: http://mathworld.wolfram.com/Ellipse.html
  double f_len = sqrt((e_context.ideal.major.len * e_context.ideal.major.len +
        e_context.ideal.minor.len * e_context.ideal.minor.len) / 4);
  double focal_factor = 2 * f_len / e_context.ideal.major.len;
  point2d_t focus_vec = {
    (stroke->pts[e_context.ideal.major.i].x - e_context.ideal.center.x)
      * focal_factor,
    (stroke->pts[e_context.ideal.major.i].y - e_context.ideal.center.y)
      * focal_factor,
  };
  e_context.result.ellipse.f1.x = e_context.ideal.center.x + focus_vec.x;
  e_context.result.ellipse.f1.y = e_context.ideal.center.y + focus_vec.y;
  e_context.result.ellipse.f2.x = e_context.ideal.center.x - focus_vec.x;
  e_context.result.ellipse.f2.y = e_context.ideal.center.y - focus_vec.y;
  e_context.result.ellipse.maj = e_context.ideal.major.len;
  e_context.result.ellipse.min = e_context.ideal.minor.len;

  return &e_context.result;
}

#undef context



////////////////////////////////////////////////////////////////////////////////
// ---------------------------- The Circle Test ----------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Make macros work for c_context.
#define context c_context

const circle_test_result_t* circle_test(const paleo_stroke_t* stroke) {
  CHECK_RTN_RESULT(e_context.stroke == stroke,
      "Ellipse not run!  Not running circle test.");
  CHECK_RTN_RESULT(stroke->closed, "Stroke not closed.");

  c_context.ideal.r = 0;
  for (int i = 0; i < stroke->num_pts; i++) {
    c_context.ideal.r += point2d_distance(
        &stroke->pts[i].p2d, &e_context.ideal.center);
  }
  c_context.ideal.r /= stroke->num_pts;

  CHECK_RTN_RESULT(stroke->ndde > PALEO_THRESH_K ||
      c_context.ideal.r < PALEO_THRESH_N,
          "NDDE (%.2f) too small for radius (%.2f)",
          stroke->ndde, c_context.ideal.r);

  CHECK_RTN_RESULT(1 - (e_context.ideal.major.len / e_context.ideal.minor.len) <
      PALEO_THRESH_O,
      "More ellipse-like: 1 - Maj(%.2f) / Min(%.2f) = %.2f >= %.2f",
      e_context.ideal.major.len, e_context.ideal.minor.len,
      1 - (e_context.ideal.major.len / e_context.ideal.minor.len),
      PALEO_THRESH_O);

  c_context.result.fa = 0;
  for (int i = 1; stroke->num_pts; i++) {
    c_context.result.fa += geom_triangle_area(
        &stroke->pts[i-1].p2d, &stroke->pts[i].p2d, &c_context.ideal.center);
  }

  double area = M_PIl * c_context.ideal.r * c_context.ideal.r;
  double fae = (c_context.result.fa = e_context.result.fa) / area;
  CHECK_RTN_RESULT(fae < PALEO_THRESH_P,
      "FA error too large: %.2f >= %.2f", fae, PALEO_THRESH_P);

  // Create beautified ideal circle & return result.
  context.result.circle.center = e_context.ideal.center;
  context.result.circle.r = context.ideal.r;
  return &c_context.result;
}

#undef context
