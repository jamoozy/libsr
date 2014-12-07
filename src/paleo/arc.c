#include <string.h>

#include "arc.h"

arc_t* arc_create(const point2d_t* p1, const point2d_t* p2,
    const point2d_t* c, double angle) {
  arc_t* self = calloc(1, sizeof(arc_t));
  arc_populate(self, p1, p2, c, angle);
  return self;
}

void arc_populate(arc_t* self, const point2d_t* p1, const point2d_t* p2,
    const point2d_t* c, double angle) {
  memcpy(&self->pts[0], p1, sizeof(point2d_t));
  memcpy(&self->pts[1], p2, sizeof(point2d_t));
  memcpy(&self->center, c, sizeof(point2d_t));
  self->angle = angle;
}

void inline arc_destroy(arc_t* self) { free(self); }
