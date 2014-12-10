#include <string.h>

#include "curve.h"

curve_t* curve_create(long num) {
  curve_t* self = calloc(1, sizeof(curve_t));
  bzero(self->pts, CURVE_CONTROL_POINT_CAP * sizeof(point2d_t));
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

static void _b(
    point2d_t* p, const point2d_t* pts, int num_pts, int i, int j, double t) {
  // First, check the params.
  assert(0 <= i && i <= j && j < num_pts);
  assert(0 <= t && t <= 1);

  // Base Case
  if (i == j) {
    memcpy(p, &pts[i], sizeof(point2d_t));
    return;
  }

  // General Case
  point2d_t a, b;
  _b(&a, pts, num_pts, i, j-1, t);
  _b(&b, pts, num_pts, i+1, j, t);

  p->x = (1 - t) * a.x + t * b.x;
  p->y = (1 - t) * a.y + t * b.y;
}

void curve_compute_points(curve_t* self, point2d_t* out, int num) {
  for (int i = 0; i < num; i++) {
    double t = i * (1.0 / (num - 1));
    curve_util_compute_point(&out[i], self->pts, self->num, t);
  }
}



////////////////////////////////////////////////////////////////////////////////
// --------------------------------- Utils ---------------------------------- //
////////////////////////////////////////////////////////////////////////////////

void curve_util_compute_point(point2d_t *p,
    const point2d_t* pts, int num_pts, double t) {
  _b(p, pts, num_pts, 0, num_pts - 1, t);
}
