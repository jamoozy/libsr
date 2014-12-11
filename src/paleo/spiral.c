#include <math.h>

#include "spiral.h"


spiral_t* spiral_create() { return malloc(sizeof(spiral_t)); }

void spiral_destroy(spiral_t* self) { free(self); }

void spiral_points(const spiral_t* self, point2d_t *pts, int n) {
  for (int i = 0; i < n; i++) {
    double t = i / (double)(n-1);
    spiral_point_at(self, &pts[i], t);
  }
}

void spiral_point_at(const spiral_t* self, point2d_t* p, double t) {
  double angle = self->theta_f - self->theta_t * (1 + self->cw * t);
  p->x = self->center.x + self->r * t * cos(angle);
  p->y = self->center.y + self->r * t * sin(angle);
}
