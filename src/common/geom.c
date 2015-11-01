/*!
 * \addtogroup common
 * \{
 *
 * \file geom.c
 * Implementation of interface defined in geom.h.
 */

#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "geom.h"
#include "point.h"



////////////////////////////////////////////////////////////////////////////////
// ----------------------- Line/Segment Intersection ------------------------ //
////////////////////////////////////////////////////////////////////////////////

/*!
 * Has to do with the intersection technique described in:
 * [http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect]
 */
typedef struct {
  double t;         //!< Vector used for intersection computation.
  double u;         //!< Vector used for intersection computation.
  char intersects;  //!< Does this even intersect?
} intersection_t;

/*!
 * Performs the intersection technique described in:
 * [http://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect]
 *
 * Computes the intersection struct for the two lines defined by a1 --> a2 and
 * b1 --> b2.  t corresponds to the a's and u to the b's.
 */
void _intersect(intersection_t* inter,
    const point2d_t* a1, const point2d_t* a2,
    const point2d_t* b1, const point2d_t* b2) {
  // Method taken from:
  // http://stackoverflow.com/questions/563198/
  //              how-do-you-detect-where-two-line-segments-intersect

  point2d_t r = { a2->x - a1->x, a2->y - a1->y };
  point2d_t s = { b2->x - b1->x, b2->y - b1->y };

  // Some pre-computing.
  const double r_s = vec_cross_prod(&r, &s);   // r x s
  point2d_t q_p;
  vec_sub(&q_p, b1, a1);                       // q - p
  double q_p_r = vec_cross_prod(&q_p, &r);

  // Rest of the computation.
  inter->t = vec_cross_prod(&q_p, &s) / r_s;
  inter->u = q_p_r / r_s;
  inter->intersects = 1;

  // Preliminary checks.
  if (GEOM_EQ_0(r_s)) {
    if (GEOM_EQ_0(q_p_r)) {
      // collinear
    } else {
      // parallel
    }
    inter->intersects = 0;
  }
}

char geom_seg_seg_intersect(const point2d_t* a1, const point2d_t* a2,
                            const point2d_t* b1, const point2d_t* b2) {
  // Compute the intersection
  intersection_t inter;
  _intersect(&inter, a1, a2, b1, b2);

  // No intersection;
  if (!inter.intersects) {
    return 0;
  }

  // Remaining checks.
  if (GEOM_IN_R(inter.t, 0, 1) && GEOM_IN_R(inter.u, 0, 1)) {
    return 1;
  }

  // Intersects the line, not the segment.
  return 0;
}

char geom_seg_line_intersect(const point2d_t* s1, const point2d_t* s2,
                             const point2d_t* l1, const point2d_t* l2) {
  // Compute the intersection
  intersection_t inter;
  _intersect(&inter, s1, s2, l1, l2);

  // No intersection;
  if (!inter.intersects) {
    return 0;
  }

  // Remaining checks.
  if (GEOM_IN_R(inter.t, 0, 1)) {
    return 1;
  }

  // Intersects the line, not the segment.
  return 0;
}

char geom_seg_line_intersection(point2d_t* isect,
    const point2d_t* s1, const point2d_t* s2,
    const point2d_t* l1, const point2d_t* l2) {
  // Compute the intersection
  intersection_t inter;
  _intersect(&inter, s1, s2, l1, l2);

  // No intersection;
  if (!inter.intersects) {
    return 1;
  }

  // Remaining checks.
  if (GEOM_IN_R(inter.t, 0, 1)) {
    // Compute the intersection point.
    isect->x = s1->x + inter.t * (s2->x - s1->x);
    isect->y = s1->y + inter.t * (s2->y - s1->y);

    // Sanity check.
    assert(isect->x == l1->x + inter.u * (l2->x - l1->x));
    assert(isect->y == l1->y + inter.u * (l2->y - l1->y));

    // ... and return.
    return 1;
  }

  // Intersects the line, not the segment.
  return 0;
}

char geom_line_line_intersection(point2d_t* isect,
    const point2d_t* s1, const point2d_t* s2,
    const point2d_t* l1, const point2d_t* l2) {
  // Compute the intersection
  intersection_t inter;
  _intersect(&inter, s1, s2, l1, l2);

  // No intersection;
  if (!inter.intersects) {
    return 1;
  }

  // Compute the intersection point.
  isect->x = s1->x + inter.t * (s2->x - s1->x);
  isect->y = s1->y + inter.t * (s2->y - s1->y);

  // Sanity check.
  assert(isect->x == l1->x + inter.u * (l2->x - l1->x));
  assert(isect->y == l1->y + inter.u * (l2->y - l1->y));

  // ... and return.
  return 1;
}



////////////////////////////////////////////////////////////////////////////////
//                              Polynomial Area                               //
////////////////////////////////////////////////////////////////////////////////

/*! Computs area of a polynomial defined by the `len` points in `pts`.
 *
 * \param pts The points in the polynomial.
 * \param len The number of points in pts.
 *
 * \return The area of the polynomial.
 */
static inline double _poly_area(const point2d_t* pts, int len) {
  assert(len > 1);

  double sum = pts[len-1].x * pts[0].y - pts[0].x * pts[len-1].y;
  for (int i = 1; i < len; i++) {
    sum += pts[i-1].x * pts[i].y - pts[i].x * pts[i-1].y;
  }

  return abs(sum) / 2;
}

double geom_triangle_area(const point2d_t* p1, const point2d_t* p2,
                          const point2d_t* p3) {
  // XXX This uses Heron's formula.  Should it?
  const double a = point2d_distance(p1, p2);
  const double b = point2d_distance(p2, p3);
  const double c = point2d_distance(p3, p1);
  const double s = (a + b + c) / 2;
  return sqrt(s * (s-a) * (s-b) * (s-c));
}

char geom_point2d_eq(const point2d_t* a, const point2d_t* b) {
  return GEOM_EQ_0(a->x - b->x) && GEOM_EQ_0(a->y - b->y);
}

double geom_quad_area(const point2d_t* p1, const point2d_t* p2,
                      const point2d_t* p3, const point2d_t* p4) {
  double area = geom_triangle_area(p1, p2, p3) + geom_triangle_area(p1, p3, p4);
  if (!(point2d_equal(p1, p3) || point2d_equal(p1, p4) ||
        point2d_equal(p2, p3) || point2d_equal(p2, p4))) {
    if (geom_seg_seg_intersect(p1, p2, p3, p4)) {
//      printf("1 --> 2  &  3 --> 4\n");
      area /= 2;
    } else if (geom_seg_seg_intersect(p1, p4, p2, p3)) {
//      printf("2 --> 3  &  1 --> 4\n");
      area /= 2;
    }
  }
  return area;
//  point2d_t pts[] = {
//    {p1->x, p1->y},
//    {p2->x, p2->y},
//    {p3->x, p3->y},
//    {p4->x, p4->y}
//  };
//  return _poly_area(pts, 4);
}

/*! \} */
