#include <strings.h>
#include <values.h>

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
  bzero(&context.ideal_ell.center, sizeof(point2d_t));
  context.ideal_ell.minor.dist = DBL_MAX;

  // Compute major and minor axes and center.
  for (int i = 0; i < stroke->num_pts; i++) {
    point2d_accum(&context.ideal_ell.center, &stroke->pts[i].p2d);

    // Loop through points 2nd time to compare all point pairs.
    for (int j = 0; j < stroke->num_pts; j++) {
      if (i == j) { continue; }  // Skip comparisons with the same point.

      // Compute dist to determine whether this pair could be the major axis.
      double dist = point2d_distance(&stroke->pts[i].p2d, &stroke->pts[j].p2d);
      if (dist > context.ideal_ell.major.dist) {
        context.ideal_ell.major.dist = dist;
        context.ideal_ell.major.i = i;
        context.ideal_ell.major.j = j;
      }
    }

    // Also compute the Yu feature area.
    if (i == 0) { continue; }
    context.result->fa += geom_triangle_area(
        &context.ideal_ell.center, &stroke->pts[i].p2d, &stroke->pts[i-1].p2d);
  }
  point2d_div(&context.ideal_ell.center, stroke->num_pts);

  // Loop through the points again to find an intersection between the stroke
  // and the perpendicular intersection of the major axis through the center.
  for (int i = 1; i < stroke->num_pts; i++) {

  }

  // Check that the NDDE is sufficiently high, or the ellipse is sufficiently
  // small.


  // Check that the feature area error (FA / ideal's area) is sufficiently
  // small.


  // If it all checks out, create the ellipse based on maj/min/center.

  return context.result;
}

const ellipse_test_result_t* circle_test(const paleo_stroke_t* stroke) {
  return context.result;
}
