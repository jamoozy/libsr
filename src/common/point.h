#ifndef __point_h__
#define __point_h__


#include <stdlib.h>
#include <math.h>

// A simple 2D point.
typedef struct {
  long x;  // X-coord.
  long y;  // Y-coord.
} point2d_t;

// A 2D point with timestamp;
typedef struct {
  long x;  // X-coord.
  long y;  // Y-coord.
  long t;  // Unix time created (for drawn points)
} point2dt_t;

// A point structure with coordinates in space, time, and index.
typedef struct {
  long x;  // X-coord.
  long y;  // Y-coord.
  long t;  // unix time created (for drawn points)
  long i;  // Index (for strokes)
} point_t;

// Creates a point.
point_t* point_create();

// Creates a point at a specific place.
//   x: The X coordinate.
//   y: The Y coordinate.
point_t* point_create_coords(long x, long y);

// Creates a timed point.
//   x: The X coordinate.
//   y: The Y coordinate.
//   t: The unix time it was created.
point_t* point_create_timed(long x, long y, long t);

// Creates a fully-populated point.
//   x: The X coordinate.
//   y: The Y coordinate.
//   t: The unix time it was created.
//   i: The index in the stroke.
point_t* point_create_full(long x, long y, long t, long i);

// Destroys (frees the memory of) a point.
void point_destroy(point_t* self);



////////////////////////////////////////////////////////////////////////////////
// ------------------------------ Point Utils ------------------------------- //
////////////////////////////////////////////////////////////////////////////////

double inline point2d_distance(const point2d_t* a, const point2d_t* b) {
  const long diff_x = a->x - b->x;
  const long diff_y = a->y - b->y;
  return sqrt(diff_x * diff_x + diff_y * diff_y);
}

#endif // __point_h__

