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

// Recursive Bézier function
void _b(curve_t* self, point2d_t* p, int i, int j, double t) {
  // First, check the params.
  assert(0 <= i && i <= j && j < self->num);
  assert(0 <= t && t <= 1);

  // Base Case
  if (i == j) {
    memcpy(p, &self->pts[i], sizeof(point2d_t));
    return;
  }

  // General Case
  point2d_t a, b;
  _b(self, &a, i, j-1, t);
  _b(self, &b, i+1, j, t);

  p->x = (1 - t) * a.x + t * b.x;
  p->y = (1 - t) * a.y + t * b.y;
}

void curve_compute_points(curve_t* self, point2d_t* out, int num) {
  for (int i = 0; i < num; i++) {
    double t = i * (1.0 / (num - 1));
    _b(self, &out[i], 0, self->num - 1, t);
  }
}

void curve_destroy(curve_t* self) {
  free(self->pts);
  free(self);
}
