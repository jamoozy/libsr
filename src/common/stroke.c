/*!
 * \addtogroup common
 * \{
 *
 * \file stroke.c
 * Implementation of interface defined in stroke.h.
 */

#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <values.h>

#include "debug.h"
#include "stroke.h"
#include "util.h"



/*! Increases the size of the array of points in the stroke.  Uses `assert()` to
 * ensure that `new_size > old size`.
 *
 * \param self The stroke to grow.
 * \param new_size The size to make the stroke.
 */
static inline void _increase_point_size_to(stroke_t* self, long new_size) {
  assert(new_size > self->size);
  self->pts = realloc(self->pts, (self->size = new_size) * sizeof(point_t));
}

//! The amount that `stroke_t->size` gets increased by when needed.
#define _PT_SZ_INC 10

/*! Increases the size of the point array if it is already full.
 * \param self The stroke to (possibly) increase.
 * \return 1 if there's enough room, 0 if there's
 */
static inline int _increase_point_size_if_full(stroke_t* self) {
  if (self->num >= self->size) {
    if (self->size > INT_MAX - _PT_SZ_INC) {
      // Overflow!
      fprintf(stderr, "Error: possible overflow!");
      return 0;
    }

    // This can cause an overflow error during compile time.  That's okay, we
    // checked for this in the previous if statement.
    _increase_point_size_to(self, self->size + _PT_SZ_INC);
  }
  return 1;
}

#undef _PT_SZ_INC


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
  if(!_increase_point_size_if_full(self)) {
    return;
  }

  memcpy(&self->pts[self->num], point, sizeof(point2dt_t));
  self->pts[self->num].i = self->num;
  self->num++;
}

void stroke_add_coords(stroke_t* self, long x, long y) {
  if(!_increase_point_size_if_full(self)) {
    return;
  }

  self->pts[self->num].x = x;
  self->pts[self->num].y = y;
  self->pts[self->num].t = get_utime();
  self->pts[self->num].i = self->num;
  self->num++;
}

void stroke_insert_at(stroke_t* self, int i, long x, long y) {
  if(!_increase_point_size_if_full(self)) {
    return;
  }

  // Shift all the points after `i`.
  memmove(&self->pts[i+1], &self->pts[i], (self->num - i) * sizeof(point_t));
  self->num++;

  // Update the point to insert.
  self->pts[i].x = x;
  self->pts[i].y = y;
  self->pts[i].t = 0;

  // Update the i's.
  for (int j = i; j < self->num; j++) {
    self->pts[j].i = j;
  }
}

void stroke_add_timed(stroke_t* self, long x, long y, long t) {
  if(!_increase_point_size_if_full(self)) {
    return;
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
    if (fprintf(fp, "%.1f,%.1f,%ld\n",
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
    if (fscanf(fp, "%lf,%lf,%ld",
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

point_t* stroke_get(const stroke_t* self, int i) {
  return &self->pts[i];
}

/*! \} */
