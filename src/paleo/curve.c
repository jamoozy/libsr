#include <string.h>

#include "curve.h"

curve_t* curve_create(long num) {
  curve_t* self = calloc(1, sizeof(curve_t));
  self->pts = calloc(self->num = num, sizeof(curve_t*));
  return self;
}

curve_t* curve_create_coords(long num, long* xs, long* ys) {
  curve_t* self = curve_create(num);
  for (int i = 0; i < self->num; i++) {
    self->pts[i].x = xs[i];
    self->pts[i].y = ys[i];
  }
  return self;
}

curve_t* curve_create_points(long num, const point2d_t* points) {
  curve_t* self = curve_create(num);
  for (int i = 0; i < self->num; i++) {
    memcpy(&self->pts[i], &points[i], sizeof(point2d_t));
  }
  return self;
}

// Destroys the curve by freeing its memory.
void curve_destroy(curve_t* self) {
  free(self->pts);
  free(self);
}
