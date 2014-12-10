#ifndef __curve_h__
#define __curve_h__

#include "common/point.h"


#define CURVE_CONTROL_POINT_CAP 5

// A Bezier curve with some number of control points.
typedef struct {
  point2d_t pts[CURVE_CONTROL_POINT_CAP];   // The control points.
  int num;                                  // The number of control points
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

// Computes 'num' evenly separated points the Bézier curve.
void curve_compute_points(curve_t* self, point2d_t* out, int num);

// Destroys the curve by freeing its memory.
static inline void curve_destroy(curve_t* self) { free(self); }

// Recursively computes a point on a Bézier curve.
//    p: The return point.
//    pts: The control points.
//    num_pts: The number of control points.
//    t: The parameter to the Bézier function.
void curve_util_compute_point(point2d_t *p,
    const point2d_t* pts, int num_pts, double t);

#endif // __curve_h__
