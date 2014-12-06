#include <config.h>
#include <strings.h>
#include <values.h>
#include <math.h>

#include "common/geom.h"
#include "test_macros.h"
#include "ellipse_test.h"



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Up & Down -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// The ellipse (and circle) context for testing.
static ellipse_test_context_t context;

void ellipse_test_init() { bzero(&context, sizeof(ellipse_test_context_t)); }

void ellipse_test_deinit() { free(context.result); }

static void _reset(const paleo_stroke_t* stroke) { context.stroke = stroke; }



////////////////////////////////////////////////////////////////////////////////
// ----------------------- The ellipsis/circle tests ------------------------ //
////////////////////////////////////////////////////////////////////////////////

const ellipse_test_result_t* ellipse_test(const paleo_stroke_t* stroke) {
  if (!stroke->closed) {
    SET_FAIL("Stroke not closed.");
    return context.result;
  }

  _reset(stroke);
  context.ellipse_run = 1;

  // Init the ideal ellipse.
  bzero(&context.ideal.center, sizeof(point2d_t));
  context.ideal.minor.len = DBL_MAX;

  // Compute major and minor axes and center.
  for (int i = 0; i < stroke->num_pts; i++) {
    point2d_accum(&context.ideal.center, &stroke->pts[i].p2d);

    // Loop through points 2nd time to compare all point pairs.
    for (int j = 0; j < stroke->num_pts; j++) {
      if (i == j) { continue; }  // Skip comparisons with the same point.

      // Compute len to determine whether this pair could be the major axis.
      double dist = point2d_distance(&stroke->pts[i].p2d, &stroke->pts[j].p2d);
      if (dist > context.ideal.major.len) {
        context.ideal.major.len = dist;
        context.ideal.major.i = i;
        context.ideal.major.j = j;
      }
    }

    // Also compute the Yu feature area.
    if (i == 0) { continue; }
    context.result->fa += geom_triangle_area(
        &context.ideal.center, &stroke->pts[i].p2d, &stroke->pts[i-1].p2d);
  }
  point2d_div(&context.ideal.center, stroke->num_pts);

  // Find line orthogonal to maj through center.
  point2d_t orth[2];
  geom_ortho_line(&orth[0], &orth[1],
      &stroke->pts[context.ideal.major.i].p2d,
      &stroke->pts[context.ideal.major.j].p2d,
      &context.ideal.center);

  // Loop through the points again to find an intersection between the stroke
  // and the perpendicular intersection of the major axis through the center.
  short which = 0;   // Keep track of which is being assigned.
  for (int i = 1; i < stroke->num_pts; i++) {
    point2d_t isect;
    if (geom_seg_line_intersection(&isect,
          &stroke->pts[i-1].p2d, &stroke->pts[i].p2d, &orth[0], &orth[1])) {
      memcpy(
          (which++ % 2) ? &context.ideal.minor.b : &context.ideal.minor.a,
          &isect, sizeof(point2d_t));
    }
  }
  assert(which >= 2);   // should have assigned at least 2 points.

  // Compute minor length.
  context.ideal.minor.len = point2d_distance(
      &context.ideal.minor.a, &context.ideal.minor.b);

  // Check that the NDDE is sufficiently high, or the ellipse is sufficiently
  // small.
  if (stroke->ndde <= PALEO_THRESH_K &&
      context.ideal.major.len >= PALEO_THRESH_L) {
    SET_FAIL("NDDE (%.2f) too small for maj length (%.2f)",
        stroke->ndde, context.ideal.major.len);
    return context.result;
  }

  // Check that the feature area error (FA / ideal's area) is sufficiently
  // small.  From the paper:
  //
  //    The feature area error (feature area divided by area of the ideal
  //    ellipse, as described in [12]) must be less than some threshold (M).
  //
  // Yu's paper (which is [12]) computes the feature area by summing all the
  // triangles defined by the stroke's sub-segments and the center point of the
  // ellipse.
  context.result->fa = 0;
  for (int i = 1; i < stroke->num_pts; i++) {
    context.result->fa += geom_triangle_area(
        &context.ideal.center, &stroke->pts[i-1].p2d, &stroke->pts[i].p2d);
  }
  double fae = 4 * context.result->fa /
    (M_PIl * context.ideal.major.len * context.ideal.minor.len);

  // If it all checks out, create the ellipse based on maj/min/center.
  //
  // This is based on: http://mathworld.wolfram.com/Ellipse.html


  return context.result;
}

const ellipse_test_result_t* circle_test(const paleo_stroke_t* stroke) {
  return context.result;
}
