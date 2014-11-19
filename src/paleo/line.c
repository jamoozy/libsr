#include <string.h>

#include "line.h"

line_t* line_create() {
  line_t* self = calloc(1, sizeof(line_t));
  self->pts = calloc(self->num = 2, sizeof(point2d_t*));
  return self;
}

line_t* line_create_points_with_points(const point2d_t* a, const point2d_t* b) {
  line_t* self = line_create();
  memcpy(&(self->pts[0]), a, sizeof(point2d_t));
  memcpy(&(self->pts[1]), b, sizeof(point2d_t));
  return self;
}

line_t* line_create_points_with_longs(long ax, long ay, long bx, long by) {
  line_t* self = line_create();
  self->pts[0].x = ax;
  self->pts[0].y = ay;
  self->pts[1].x = bx;
  self->pts[1].y = by;
  return self;
}

void line_destroy(line_t* self) {
  free(self->pts);
  free(self);
}
