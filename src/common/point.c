/*! @file point.c
 * Implementation of interface defined in point.h.
 */

#include "point.h"

point_t* point_create() {
  point_t* self = calloc(1, sizeof(point_t));
  self->t = self->i = -1;
  return self;
}

point_t* point_create_coords(long x, long y) {
  point_t* self = point_create();
  self->x = x;
  self->y = y;
  return self;
}

point_t* point_create_timed(long x, long y, long t) {
  point_t* self = point_create_coords(x, y);
  self->t = t;
  return self;
}

point_t* point_create_full(long x, long y, long t, long i) {
  point_t* self = point_create_timed(x, y, t);
  self->i = i;
  return self;
}

void inline point_destroy(point_t* self) {
  free(self);
}

void point2d_bis(point2d_t* o1, point2d_t* o2,
    const point2d_t* a, const point2d_t* b) {
  point2d_t c;
  point2d_center(&c, a, b);

  // vec from c to b
  point2d_t cb = { b->x - c.x, b->y - c.y };

  // The output points.
  o1->x = c.x + cb.y;
  o1->y = c.y - cb.x;
  o2->x = c.x - cb.y;
  o2->y = c.y + cb.x;
}
