#ifndef __point_h__
#define __point_h__


#include <stdlib.h>

// A point structure with coordinates in space, time, and index.
typedef struct {
  long x;  // X-coord.
  long y;  // Y-coord.
  long t;  // unix time created (for drawn points)
  long i;  // Index (for strokes
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
void inline point_destroy(point_t* self) { free(self); }

#endif // __point_h__

