#include <string.h>

#include "arc.h"

arc_t* arc_create(const point2d_t* e1, const point2d_t* e2, const point2d_t* c,
    char cw) {
  arc_t* self = calloc(1, sizeof(arc_t));
  memcpy(&self->endpts[0], e1, sizeof(point2d_t));
  memcpy(&self->endpts[1], e2, sizeof(point2d_t));
  memcpy(&self->center, c, sizeof(point2d_t));
  self->cw = cw;
  return self;
}

void inline arc_destroy(arc_t* self) { free(self); }
