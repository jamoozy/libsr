#include "curve.h"

curve_t* curve_create(long num) {
  curve_t* self = calloc(1, sizeof(curve_t));
  self->num = num;
  self->points = calloc(self->num, sizeof(curve_t*));
  return self;
}

curve_t* curve_create_coords(long num, long* xs, long* ys) {
  curve_t* self = curve_create(num);
  for (int i = 0; i < self->num; i++) {
    self->points[i] = point_create(xs[i], ys[i]);
  }
  return self;
}

curve_t* curve_create_points(long num, point_t** points) {
  curve_t* self = curve_create(num);
  for (int i = 0; i < self->num; i++) {
    self->points[i] = points[i];
  }
  return self;
}

// Destroys the curve by freeing its memory.
void curve_destroy(curve_t* self) {
  for (int i = 0; i < self->num; i++) {
    free(self->points[i]);
  }
  free(self->points);
  free(self);
}
