#include <config.h>
#include <values.h>
#include <math.h>

#include "common/geom.h"
#include "common/util.h"
#include "thresh.h"
#include "test_macros.h"
#include "helix_test.h"


static paleo_helix_test_context_t context;



void paleo_helix_test_init() {
  bzero(&context, sizeof(paleo_helix_test_context_t));
}

void paleo_helix_test_deinit() { }

void _reset(const paleo_stroke_t* stroke) {
  bzero(&context, sizeof(paleo_helix_test_context_t));
  context.stroke = stroke;
}



const paleo_helix_test_result_t*
paleo_helix_test(const paleo_stroke_t* stroke) {
  _reset(stroke);

  CHECK_RTN_RESULT(stroke->overtraced, "Stroke not overtraced.");
  CHECK_RTN_RESULT(stroke->ndde > PALEO_THRESH_K,
      "NDDE too low: %.2f <= K (%.2f)", stroke->ndde, PALEO_THRESH_K);

  // Check that this looks helix-like.
  double ep_dist = point2d_distance(
      &stroke->pts[0].p2d, &stroke->pts[stroke->num_pts-1].p2d);
  CHECK_RTN_RESULT(ep_dist / stroke->px_length >= PALEO_THRESH_U,
      "ep_dist (%.2f) / px_len (%.2f) >= U (%.2f)",
      ep_dist, stroke->px_length, PALEO_THRESH_U);

  // All tests pass, build the helix.

  // Compute "major axis"
  struct {
    int i;
    int j;
    double dist;
    double angle;
  } maj = { -1, -1, DBL_MIN };
  for (int i = 0; i < stroke->num_pts; i++) {
    for (int j = i+1; j < stroke->num_pts; j++) {
      double dist = point2d_distance(&stroke->pts[i].p2d, &stroke->pts[j].p2d);
      if (dist > maj.dist) {
        maj.i = i;
        maj.j = j;
        maj.dist = dist;
      }
    }
  }
  maj.angle = point2d_angle_to(
      &stroke->pts[maj.i].p2d, &stroke->pts[maj.j].p2d);

  // Compute radius.
  context.ideal.r = 0;
  for (int i = 0; i < stroke->num_pts; i++) {
    context.ideal.r += geom_point_dist_to_line(
        &stroke->pts[i].p2d, &stroke->pts[maj.i].p2d, &stroke->pts[maj.j].p2d);
  }
  context.ideal.r /= stroke->num_pts;

  point2d_t vec = {
    stroke->pts[maj.j].x - stroke->pts[maj.i].x,
    stroke->pts[maj.j].y - stroke->pts[maj.i].y
  };
  context.result.helix.c[0].x =
    stroke->pts[maj.i].x + vec.x * context.ideal.r / maj.dist;
  context.result.helix.c[0].y =
    stroke->pts[maj.i].x + vec.y * context.ideal.r / maj.dist;
  context.result.helix.c[1].x =
    stroke->pts[maj.j].x - vec.x * context.ideal.r / maj.dist;
  context.result.helix.c[1].y =
    stroke->pts[maj.j].x - vec.y * context.ideal.r / maj.dist;
  context.result.helix.r = context.ideal.r;
  context.result.helix.theta_i = point2d_angle_to(
      &context.result.helix.c[0], &stroke->pts[0].p2d);
  context.result.helix.theta_t =
    stroke->pts[stroke->num_pts-1].dir - stroke->pts[0].dir;
  context.result.helix.cw = SGN(context.result.helix.theta_t);
  context.result.helix.theta_t = abs(context.result.helix.theta_t);

  return &context.result;
}
