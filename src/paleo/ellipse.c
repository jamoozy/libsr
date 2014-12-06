#include <string.h>

#include "ellipse.h"

ellipse_t* ellipse_create(
    const point2d_t* f1, const point2d_t* f2, double maj, double min) {
  ellipse_t* self = calloc(1, sizeof(ellipse_t));
  ellipse_populate(self, f1, f2, maj, min);
  return self;
}

void ellipse_populate(ellipse_t* self,
    const point2d_t* f1, const point2d_t* f2, double maj, double min) {
  memcpy(&self->f1, f1, sizeof(point2d_t));
  memcpy(&self->f2, f2, sizeof(point2d_t));
  self->maj = maj;
  self->min = min;
}

void inline ellipse_destroy(ellipse_t* self) { free(self); }
