#include <config.h>
#include <strings.h>
#include <values.h>
#include <math.h>

#include "common/util.h"
#include "common/point.h"
#include "common/geom.h"
#include "test_macros.h"
#include "arc_test.h"



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Up & Down -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// The arc context for testing.
static arc_test_context_t context;

void arc_test_init() { bzero(&context, sizeof(arc_test_context_t)); }

void arc_test_deinit() { free(context.result); }

static inline void _reset(const paleo_stroke_t* stroke) {
  void* result = context.result;
  bzero(&context, sizeof(arc_test_context_t));
  context.result = realloc(result, sizeof(arc_test_result_t));
  context.stroke = stroke;
  context.result->possible = 1;
}



////////////////////////////////////////////////////////////////////////////////
// ------------------------------ The Arc Test ------------------------------ //
////////////////////////////////////////////////////////////////////////////////

const arc_test_result_t* arc_test(const paleo_stroke_t* stroke) {
  CHECK_RTN_RESULT(!stroke->closed, "Stroke closed.");
  CHECK_RTN_RESULT(!stroke->overtraced, "Stroke overtraced.");
  CHECK_RTN_RESULT(stroke->dcr < PALEO_THRESH_J,
      "DCR (%.2f) >= J (%.2f)", stroke->dcr, PALEO_THRESH_J);

  _reset(stroke);

  // Paulson fits arcs by modeling them as parts of circles.  From the article:
  //
  //    ... we consider arcs to be segments of circles; therefore, ... we need
  //    to determine the best fit circle that the arc is a part of ... [by
  //    calculating] the ideal center point of the arc by using a series of
  //    perpendicular bisectors.
  //
  // Here we go! (as specified by the paper)

  point2d_t ep_pbis[2];    // endpoint perpendicular bisector
  point2d_bis(&ep_pbis[0], &ep_pbis[1],
      &stroke->pts[0].p2d, &stroke->pts[stroke->num_pts-1].p2d);

  // Find perp. bis. with stroke.  Call this the "center bisector".
  point2d_t bis_isect;    // Bisector's intersection.
  for (int i = 1; i < stroke->num_pts; i++) {
    if (geom_seg_line_intersection(&bis_isect, &stroke->pts[i-1].p2d,
          &stroke->pts[i].p2d, &ep_pbis[0], &ep_pbis[1])) {
      break;    // Found an intersection, so stop looking.
    }
  }

  // Find perp bisectors of the segments formed with the endpoints and the
  // "center bisector".
  point2d_t pbis[4];
  point2d_bis(&pbis[0], &pbis[1], &stroke->pts[0].p2d, &bis_isect);
  point2d_bis(&pbis[2], &pbis[3], &bis_isect,
      &stroke->pts[stroke->num_pts-1].p2d);

  // Find their intersection and call it the center of the arc's circle.
  geom_line_line_intersection(&context.ideal.center,
      &pbis[0], &pbis[1], &pbis[2], &pbis[3]);

  // Calculate the radius by finding the average distance to a stroke point.
  context.ideal.r = 0;
  for (int i = 0; i < stroke->num_pts; i++) {
    context.ideal.r += point2d_distance(
        &context.ideal.center, &stroke->pts[i].p2d);
  }
  context.ideal.r /= stroke->num_pts;

  CHECK_RTN_RESULT(stroke->ndde > PALEO_THRESH_K ||
      context.ideal.r < PALEO_THRESH_N,
          "NDDE (%.2f) <= K (%.2f) || r (%.2f) >= N (%.2f)",
              stroke->ndde, PALEO_THRESH_K, context.ideal.r, PALEO_THRESH_N);

  // Compute angle traversed to compute the ideal area.
  double angle = 0;
  for (int i = 1; i < stroke->num_pts; i++) {
    double d_angle = point2d_angle_to(
        &context.ideal.center, &stroke->pts[i].p2d) -
      point2d_angle_to(&context.ideal.center, &stroke->pts[i-1].p2d);
    NORM_ANGLE(d_angle);
    angle += d_angle;
  }
  context.ideal.area = angle * context.ideal.r * context.ideal.r / 2;

  // Compute simple (Yu) feature area.
  context.result->fa = 0;
  for (int i = 1; i < stroke->num_pts; i++) {
    context.result->fa =+ geom_triangle_area(
        &context.ideal.center, &stroke->pts[i-1].p2d, &stroke->pts[i].p2d);
  }

  // Compute Paulson FA error.
  context.result->fae = context.result->fa / context.ideal.area;
  CHECK_RTN_RESULT(context.result->fae < PALEO_THRESH_Q,
      "FAE (%.2f) >= Q (%.2f)", context.result->fae, PALEO_THRESH_Q);

  // Everything checks out!  Populate the arc and return the result.
  arc_populate(&context.result->arc, &stroke->pts[0].p2d, &stroke->pts[1].p2d,
      &context.ideal.center, angle);
  return context.result;
}
