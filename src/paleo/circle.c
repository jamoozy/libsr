#include <string.h>

#include "circle.h"

circle_t* circle_create() {
  circle_t* self = calloc(1, sizeof(circle_t));
  return self;
}

circle_t* circle_create_full(long r, long x, long y) {
  circle_t* self = circle_create();
  self->r = r;
  self->c.x = x;
  self->c.y = y;
  return self;
}

circle_t* circle_create_with_point(long r, const point2d_t* c) {
  circle_t* self = circle_create();
  self->r = r;
  memcpy(&self->c, c, sizeof(point2d_t));
  return self;
}

void inline circle_destroy(circle_t* self) { free(self); }
