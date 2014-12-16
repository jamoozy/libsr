#include <string.h>
#include <strings.h>

#include "composite.h"


static paleo_composite_context_t context;

void paleo_composite_init() {
  bzero(&context, sizeof(paleo_composite_context_t));
}

void paleo_composite_deinit() { }

static void _reset(const paleo_stroke_t* stroke) {
  bzero(&context, sizeof(paleo_composite_context_t));
  context.stroke = stroke;
  context.result.possible = 1;
}



static void _rerun(int x) {
  stroke_t stroke[2];

  stroke[0].pts = calloc(stroke[0].num = stroke[0].size = x, sizeof(point_t));
  for (int i = 0; i < x; i++) {
    memcpy(&stroke[0].pts[i], &context.stroke->pts[i], sizeof(point_t));
  }

  stroke[1].num = context.stroke->num_pts - x;
  stroke[1].pts = calloc(stroke[1].num, sizeof(point_t));
  for (int i = x; i < context.stroke->num_pts; i++) {
    memcpy(&stroke[1].pts[i], &context.stroke->pts[i+x], sizeof(point_t));
    stroke[1].pts[i].i = i;
  }
}

const paleo_composite_result_t*
paleo_composite_test(const paleo_stroke_t* stroke) {
  _reset(stroke);


}
