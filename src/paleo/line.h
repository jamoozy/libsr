#ifndef __line_h__
#define __line_h__

#include "point.h"

// A line with some number of joints.
typedef struct {
  size_t num;       // The number of points, including start and end.
  point_t** points;  // The points.  Must be at least 2.
} line_t;

// Creates a new line from 0,0 to 0,0.
line_t* line_create();

// Creates a line with the given points.  The line will then grab ownership of
// the points, meaning line_destroy(line_t*) will free the passed points.
//   a: The starting point of the line.
//   b: The ending point of the line.
line_t* line_create_points_with_points(point_t* a, point_t* b);

// Creates a line containing newly created points with the given coordinates.
//   ax: The starting point's X coord.
//   ay: The starting point's Y coord.
//   bx: The ending point's X coord.
//   by: The ending point's Y coord.
line_t* line_create_points_with_longs(long ax, long ay, long bx, long by);

// Frees the memory in a line.
//   self: The line to free.
void line_destroy(line_t* self);

#endif // __line_h__
