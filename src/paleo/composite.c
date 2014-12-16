#include <string.h>
#include <strings.h>

#include "composite.h"


void paleo_composite_init() {
}

void paleo_composite_deinit() { }

static void _reset(
    paleo_composite_context_t* context, const paleo_stroke_t* stroke) {
  bzero(context, sizeof(paleo_composite_context_t));
  context->stroke = stroke;
  context->result.possible = 1;
}



paleo_composite_result_t* paleo_composite_test(const paleo_stroke_t* stroke) {
  assert(0);  // still have to figure out how to implement this ...

  paleo_composite_context_t context;
  bzero(&context, sizeof(paleo_composite_context_t));
  _reset(&context, stroke);

  // Find highest curvature.
  paleo_point_t* max = &stroke->pts[0];
  for (int i = 1; i < stroke->num_pts; i++) {
    if (stroke->pts[i].curv > max->curv) {
      max = &stroke->pts[i];
    }
  }

  // Break into 2 substrokes.
  stroke_t subs[2];
  subs[0].pts = calloc(subs[0].num = subs[0].size = max->i, sizeof(point_t));
  for (int i = 0; i < max->i; i++) {
    memcpy(&subs[0].pts[i], &stroke->pts[i], sizeof(point_t));
  }

  subs[1].num = stroke->num_pts - max->i;
  subs[1].pts = calloc(subs[1].num, sizeof(point_t));
  for (int i = max->i; i < stroke->num_pts; i++) {
    memcpy(&subs[1].pts[i], &stroke->pts[i+max->i], sizeof(point_t));
    subs[1].pts[i].i = i;
  }

  // Test each individually.
  &context, paleo_recognize(&subs[0]);
  &context, paleo_recognize(&subs[1]);

  // 
}

static void _split(stroke_t stroke[2], int x) {
}

