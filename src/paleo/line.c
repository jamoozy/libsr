#include "line.h"

line_t* line_create() {
  line_t* self = calloc(1, sizeof(line_t));
  self->num = 2;
  self->points = calloc(2, sizeof(point_t*));
  return self;
}

line_t* line_create_points_with_points(point_t* a, point_t* b) {
  line_t* self = line_create();
  self->points[0] = a;
  self->points[1] = b;
  return self;
}

line_t* line_create_points_with_longs(long ax, long ay, long bx, long by) {
  line_t* self = line_create();
  self->points[0] = point_create_coords(ax, ay);
  self->points[1] = point_create_coords(bx, by);
  return self;
}

void line_destroy(line_t* self) {
  for (int i = 0; i < self->num; i++) {
    point_destroy(self->points[i]);
  }
  free(self->points);
  free(self);
}
