/*! \addtogroup pal
 * \{
 *
 * \file composite.c
 * Implements the interface defined in composite.h.
 */

#include <string.h>
#include <strings.h>

#include "composite.h"
#include "test_macros.h"


void pal_composite_cpy(pal_composite_t* dst, const pal_composite_t* src) {
  dst->subs = calloc(dst->num_subs = src->num_subs, sizeof(pal_sub_shape_t));
  for (int i = 0; i < src->num_subs; i++) {
    dst->subs[i].type = src->subs[i].type;
    switch (src->subs[i].type) {
      case PAL_TYPE_LINE:
        dst->subs[i].shape = malloc(sizeof(pal_line_t));
        pal_line_cpy(dst->subs[i].shape, src->subs[i].shape);
        break;

      case PAL_TYPE_ELLIPSE:
        dst->subs[i].shape = malloc(sizeof(pal_ellipse_t));
        pal_ellipse_cpy(dst->subs[i].shape, src->subs[i].shape);
        break;

      case PAL_TYPE_CIRCLE:
        dst->subs[i].shape = malloc(sizeof(pal_circle_t));
        pal_circle_cpy(dst->subs[i].shape, src->subs[i].shape);
        break;

      case PAL_TYPE_ARC:
        dst->subs[i].shape = malloc(sizeof(pal_arc_t));
        pal_arc_cpy(dst->subs[i].shape, src->subs[i].shape);
        break;

      case PAL_TYPE_CURVE:
        dst->subs[i].shape = malloc(sizeof(pal_curve_t));
        pal_curve_cpy(dst->subs[i].shape, src->subs[i].shape);
        break;

      case PAL_TYPE_SPIRAL:
        dst->subs[i].shape = malloc(sizeof(pal_spiral_t));
        pal_spiral_cpy(dst->subs[i].shape, src->subs[i].shape);
        break;

      case PAL_TYPE_HELIX:
        dst->subs[i].shape = malloc(sizeof(pal_helix_t));
        pal_helix_cpy(dst->subs[i].shape, src->subs[i].shape);
        break;

      case PAL_TYPE_COMPOSITE:
        dst->subs[i].shape = malloc(sizeof(pal_composite_t));
        pal_composite_cpy(dst->subs[i].shape, src->subs[i].shape);
        break;

      default:
        fprintf(stderr, "Got bad type: %d", src->subs[i].type);
        break;
    }
  }
}

int pal_composite_rank(const pal_composite_t* self) {
  int rank = 0;
  for (int i = 0; i < self->num_subs; i++) {
    rank += pal_shape_rank(self->subs[i].type, self->subs[i].shape);
  }
  return rank;
}

int pal_composite_is_line(const pal_composite_t* self) {
  for (int i = 0; i < self->num_subs; i++) {
    if (self->subs[i].type == PAL_TYPE_ELLIPSE ||
        self->subs[i].type == PAL_TYPE_CIRCLE ||
        self->subs[i].type == PAL_TYPE_ARC ||
        self->subs[i].type == PAL_TYPE_CURVE ||
        self->subs[i].type == PAL_TYPE_SPIRAL ||
        self->subs[i].type == PAL_TYPE_HELIX) {
      return 0;
    }
    if (self->subs[i].type == PAL_TYPE_COMPOSITE &&
        !pal_composite_is_line((pal_composite_t*)self->subs[i].shape)) {
      return 0;
    }
  }
  return 1;
}



void pal_composite_init() { }

void pal_composite_deinit() { }



pal_composite_result_t* pal_composite_test(const pal_stroke_t* stroke) {
  pal_composite_context_t context;
  bzero(&context, sizeof(pal_composite_context_t));
  RESET(stroke);

  // Find highest curvature.
  pal_point_t* max = &stroke->pts[0];
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
  &context, pal_recognize(&subs[0]);
  &context, pal_recognize(&subs[1]);

  assert(0);  // still have to figure out how to implement this ...

  // - TODO -
  // write me


  // - TODO -
  // Check for extra composite "shape tails" and rank the composite sans the
  // these to see if it truly belongs at the top.
  //
  // This was originally meant as a step after the hierarchy, but I don't see
  // why it shouldn't be put here, so here it goes.
}

/*! Splits the stroke into 2 separate substrokes and re-runs the composite test.
 *
 * \param stroke
 * \param x
 */
static void _split(stroke_t stroke[2], int x) {
}

