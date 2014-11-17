#include <assert.h>
#include <string.h>

#include "stroke.h"
#include "util.h"



// Increases the size of the array of points in the stroke.  Usees assert.h to
// ensure that new_size > old size.
static inline void _increase_point_size_to(stroke_t* self, long new_size) {
  assert(new_size > self->size);
  self->points = realloc(self->points, (self->size = new_size) * sizeof(point_t));
}


stroke_t* stroke_create(int size) {
  stroke_t* self = calloc(1, sizeof(stroke_t));
  self->points = calloc(self->size = size, sizeof(point_t));
  return self;
}

stroke_t* stroke_create_point2dts(int size, point2dt_t** points) {
  stroke_t* self = stroke_create(size);
  for (; self->num < self->size; self->num++) {
    memcpy(&self->points[self->num], points[self->num], sizeof(point2dt_t));
    self->points[self->num].i = self->num;
  }
  return self;
}

void stroke_add_point2dt(stroke_t* self, point2dt_t* point) {
  if (self->num >= self->size) {
    _increase_point_size_to(self, self->size + 10);
  }

  memcpy(&self->points[self->num], point, sizeof(point2dt_t));
  self->points[self->num].i = self->num;
  self->num++;
}

void stroke_add_coords(stroke_t* self, long x, long y) {
  if (self->num >= self->size) {
    _increase_point_size_to(self, self->size + 10);
  }

  self->points[self->num].x = x;
  self->points[self->num].y = y;
  self->points[self->num].t = get_utime();
  self->points[self->num].i = self->num;
  self->num++;
}

void stroke_add_timed(stroke_t* self, long x, long y, long t) {
  if (self->num >= self->size) {
    _increase_point_size_to(self, self->size + 10);
  }

  self->points[self->num].x = x;
  self->points[self->num].y = y;
  self->points[self->num].t = t;
  self->points[self->num].i = self->num;
  self->num++;
}
