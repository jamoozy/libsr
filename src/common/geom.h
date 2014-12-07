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
static inline double vec_cross_prod(const point2d_t* u, const point2d_t* v) {
  return u->x * v->y - v->x * u->y;
}

// Computes rtn = u (cross) v
static inline void vec_sub(point2d_t* rtn, const point2d_t* u, const point2d_t* v) {
  rtn->x = u->x - v->x;
  rtn->y = u->y - v->y;
}



// Determines if the line a1 -> a2 intersects the line b1 -> b2.
char geom_seg_seg_intersect(const point2d_t* a1, const point2d_t* a2,
                            const point2d_t* b1, const point2d_t* b2);

// Determines if the line segment s1 -> s2 intersects the infinite line defined
// by l1 -> l2.
//   s1: First point in line segment.
//   s2: Second point in line segment.
//   l1: A point in the (infinitely long) line.
//   l2: A point in the (infinitely long) line.
char geom_seg_line_intersect(const point2d_t* s1, const point2d_t* s2,
                             const point2d_t* l1, const point2d_t* l2);

// Computes the intersection point between the line segment defined by s1 -> s2
// and the infinite line defined by l1 -> l2.
// infinite line's.
//   isect: Return intersection point.
//   s1: First point in line segment.
//   s2: Second point in line segment.
//   l1: A point in the (infinitely long) line.
//   l2: A point in the (infinitely long) line.
// Returns 1 on success and 0 on failure.
char geom_seg_line_intersection(point2d_t* isect,
    const point2d_t* s1, const point2d_t* s2,
    const point2d_t* l1, const point2d_t* l2);

// Finds the intersection between 2 infinitely long lines.
//   isect: Return intersection point.
//   s1: First point in first (infinitely long) line.
//   s2: Second point in first (infinitely long) line.
//   l1: A point in the (infinitely long) line.
//   l2: A point in the (infinitely long) line.
char geom_line_line_intersection(point2d_t* isect,
    const point2d_t* s1, const point2d_t* s2,
    const point2d_t* l1, const point2d_t* l2);



// Computes area of triangle.
double geom_triangle_area(const point2d_t* p1, const point2d_t* p2,
                          const point2d_t* p3);

// Computes the area of an arbitrary quadrilateral defined by a,b,c,d.  The
// order of the point determines the shape of the quadrilateral -- a is
// connected to be and d, b is connected to a and c, and so on.  Should be able
// to handle "folded" quads.
double geom_quad_area(const point2d_t* p1, const point2d_t* p2,
                      const point2d_t* p3, const point2d_t* p4);


// Computes two points defining the line orthogonal to the input points through
// the center point.
//   out1: Output point #1.
//   out2: Output point #2.
//   in1: Input point #1.
//   in2: Input point #2.
//   c: Center point to create output line through.
static inline void geom_ortho_line(point2d_t* out1, point2d_t* out2,
    const point2d_t* in1, const point2d_t* in2, const point2d_t* c) {
  out1->x = in1->y - c->y + c->x;
  out2->y = in2->x - c->x + c->y;
}

#endif  // __common_geom_h__

