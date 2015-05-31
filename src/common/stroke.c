#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "stroke.h"
#include "util.h"



// Increases the size of the array of points in the stroke.  Usees assert.h to
// ensure that new_size > old size.
static inline void _increase_point_size_to(stroke_t* self, long new_size) {
  assert(new_size > self->size);
  self->pts = realloc(self->pts, (self->size = new_size) * sizeof(point_t));
}


stroke_t* stroke_create(int size) {
  stroke_t* self = calloc(1, sizeof(stroke_t));
  self->pts = calloc(self->size = size, sizeof(point_t));
  return self;
}

stroke_t* stroke_create_point2dts(int size, const point2dt_t* pts) {
  stroke_t* self = stroke_create(size);
  for (; self->num < self->size; self->num++) {
    memcpy(&self->pts[self->num], &pts[self->num], sizeof(point2dt_t));
    self->pts[self->num].i = self->num;
  }
  return self;
}

void stroke_add_point2dt(stroke_t* self, const point2dt_t* point) {
  if (self->num >= self->size) {
    _increase_point_size_to(self, self->size + 10);
  }

  memcpy(&self->pts[self->num], point, sizeof(point2dt_t));
  self->pts[self->num].i = self->num;
  self->num++;
}

void stroke_add_coords(stroke_t* self, long x, long y) {
  if (self->num >= self->size) {
    _increase_point_size_to(self, self->size + 10);
  }

  self->pts[self->num].x = x;
  self->pts[self->num].y = y;
  self->pts[self->num].t = get_utime();
  self->pts[self->num].i = self->num;
  self->num++;
}

void stroke_add_timed(stroke_t* self, long x, long y, long t) {
  if (self->num >= self->size) {
    _increase_point_size_to(self, self->size + 10);
  }

  self->pts[self->num].x = x;
  self->pts[self->num].y = y;
  self->pts[self->num].t = t;
  self->pts[self->num].i = self->num;
  self->num++;
}

int stroke_save(stroke_t* self, const char* fname) {
  FILE* fp = fopen(fname, "w");
  if (!fp) {
    fprintf(stderr, "Could not write to file: %s", fname);
    return 0;
  }

  if (fprintf(fp, "%ld\n", self->num) < 0) {
    fclose(fp);
    return 0;
  }
  for (int i = 0; i < self->num; i++) {
    if (fprintf(fp, "%ld,%ld,%ld\n",
                self->pts[i].x, self->pts[i].y, self->pts[i].t) < 0) {
      fclose(fp);
      return 0;
    }
  }
  fclose(fp);
  return 1;
}

stroke_t* stroke_from_file(const char* fname) {
  FILE* fp = fopen(fname, "r");
  if (!fp) {
    fprintf(stderr, "Could not read from file: %s", fname);
    return NULL;
  }

  stroke_t* self = calloc(sizeof(stroke_t), 1);
  if (fscanf(fp, "%ld\n", &self->num) == EOF) {
    stroke_destroy(self);
    fclose(fp);
    return NULL;
  }

  self->pts = calloc(sizeof(point_t), self->num);
  for (int i = 0; i < self->num; i++) {
    if (fscanf(fp, "%ld,%ld,%ld",
               &self->pts[i].x, &self->pts[i].y, &self->pts[i].t) == EOF) {
      stroke_destroy(self);
      fclose(fp);
      return NULL;
    }
    self->pts[i].i = i;
  }
  fclose(fp);
  return self;
}
