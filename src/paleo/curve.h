#ifndef __curve_h__
#define __curve_h__

#include "common/point.h"

// A Bezier curve with some number of control points.
typedef struct {
  long num;        // The number of control points
  point2d_t* pts;  // The control points.
} curve_t;

// Creates a new curve at with the specified number of points, all of which
// are at 0,0.
//   num: The number of points in the curve.
curve_t* curve_create(long num);

// Creates a curve with newly created points at the specified coordinates.
//   num: The number of points in the curve.
//   xs: X coordinates.
//   ys: Y coordinates.
curve_t* curve_create_coords(long num, long* xs, long* ys);

// Creates a curve with the given points.
//   num: The number of points.
//   points: The points.
curve_t* curve_create_points(long num, const point2d_t* points);

// Destroys the curve by freeing its memory.
void curve_destroy(curve_t*);

#endif // __curve_h__
