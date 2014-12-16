
#include <math.h>

#include "helix.h"


helix_t* helix_create() { return calloc(1, sizeof(helix_t)); }

void helix_destroy(helix_t* self) { free(self); }

void helix_compute_points(const helix_t* self, point2d_t* p, int num) {
  for (int i = 0; i < num; i++) {
    helix_compute_point(self, &p[i], i / (num-1.0));
  }
}

void helix_compute_point(const helix_t* self, point2d_t* p, double t) {
  double a = self->theta_i + self->cw * self->theta_t * t;
  p->x = self->c[0].x * (1-t) + self->c[1].x * t + self->r * cos(a);
  p->y = self->c[0].y * (1-t) + self->c[1].y * t + self->r * sin(a);
}
