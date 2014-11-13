#include "circle.h"

circle_t* circle_create() {
  circle_t* self = calloc(1, sizeof(circle_t));
  self->c = point_create();
  return self;
}

circle_t* circle_create_full(long r, long x, long y) {
  circle_t* self = circle_create();
  self->r = r;
  self->c->x = x;
  self->c->y = y;
  return self;
}

circle_t* circle_create_with_point(long r, point_t* c) {
  circle_t* self = circle_create();
  self->r = r;
  self->c = c;
  return self;
}

void circle_destroy(circle_t* self) {
  free(self->c);
  free(self);
}
