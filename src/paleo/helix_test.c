#include <config.h>
#include <values.h>
#include <math.h>

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
  CHECK_RTN_RESULT(stroke->overtraced, "Stroke not overtraced.");
  CHECK_RTN_RESULT(stroke->ndde > PALEO_THRESH_K,
      "NDDE too low: %.2f <= K (%.2f)", stroke->ndde, PALEO_THRESH_K);

  // Check that this looks helix-like.
  double ep_dist = point2d_distance(
      &stroke->pts[0].p2d, &stroke->pts[stroke->num_pts-1].p2d);
  CHECK_RTN_RESULT(ep_dist / stroke->px_length >= PALEO_THRESH_U,
      "ep_dist (%.2f) / px_len (%.2f) >= U (%.2f)",
      ep_dist, stroke->px_length, PALEO_THRESH_U);

  _reset(stroke);



  return &context.result;
}
