#include <config.h>
#include <strings.h>
#include <values.h>
#include <math.h>

#include "common/point.h"
#include "common/geom.h"
#include "test_macros.h"
#include "arc_test.h"



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Up & Down -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// The ellipse context for testing.
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
// --------------------------- The Ellipsis Test ---------------------------- //
////////////////////////////////////////////////////////////////////////////////

const arc_test_result_t* ellipse_test(const paleo_stroke_t* stroke) {
  // Extra bit -- stroke shouldn't be closed.
  CHECK_RTN_RESULT(!stroke->closed, "Stroke closed.");

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

  return context.result;
}
