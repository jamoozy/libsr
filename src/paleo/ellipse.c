#include <string.h>

#include "ellipse.h"

ellipse_t* ellipse_create() {
  ellipse_t* self = calloc(1, sizeof(ellipse_t));
  return self;
}

ellipse_t* ellipse_create_full(long r, long x, long y) {
  ellipse_t* self = ellipse_create();
  self->r = r;
  self->c.x = x;
  self->c.y = y;
  return self;
}

ellipse_t* ellipse_create_with_point(long r, const point2d_t* c) {
  ellipse_t* self = ellipse_create();
  self->r = r;
  memcpy(&self->c, c, sizeof(point2d_t));
  return self;
}

void inline ellipse_destroy(ellipse_t* self) { free(self); }
