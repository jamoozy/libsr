#ifndef __common_geom_h__
#define __common_geom_h__

#include <math.h>

#include "point.h"

// Error used to handle number rounding errors.
#define GEOM_ERR 0.00000001

// Determines if the value is low enough to be considered 0.
#define GEOM_EQ_0(x) (abs(x) <= GEOM_ERR)

// Determines if a number is in a range.
#define GEOM_IN_R(x,a,z) ((a) - GEOM_ERR < (x) && (x) < (z) + GEOM_ERR)

// Finds the cross-product of the two lines.
inline double vec_cross_prod(const point2d_t* u, const point2d_t* v) {
  return u->x * v->y - v->x * u->y;
}

// Computes rtn = u (cross) v
inline void vec_sub(point2d_t* rtn, const point2d_t* u, const point2d_t* v) {
  rtn->x = u->x - v->x;
  rtn->y = u->y - v->y;
}



// Determines if the line a1 -> a2 intersects the line b1 -> b2.
char geom_segs_intersect(const point2d_t* a1, const point2d_t* a2,
                         const point2d_t* b1, const point2d_t* b2);



// Computes area of triangle using Heron's Formula.
double geom_triangle_area(const point2d_t* p1, const point2d_t* p2,
                          const point2d_t* p3);

// Computes the area of an arbitrary quadrilateral defined by a,b,c,d.  The
// order of the point determines the shape of the quadrilateral -- a is
// connected to be and d, b is connected to a and c, and so on.  Should be able
// to handle "folded" quads.
double geom_quad_area(const point2d_t* p1, const point2d_t* p2,
                      const point2d_t* p3, const point2d_t* p4);

#endif  // __common_geom_h__

