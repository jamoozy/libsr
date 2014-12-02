#include <math.h>

#include "geom.h"
#include "point.h"


double
geom_triangle_area(const point2d_t* p1, const point2d_t* p2,
                   const point2d_t* p3) {
  const double a = point2d_distance(p1, p2);
  const double b = point2d_distance(p2, p3);
  const double c = point2d_distance(p3, p1);
  const double s = (a + b + c) / 2;
  return sqrt(s * (s-a) * (s-b) * (s-c));
}

double
geom_quad_area(const point2d_t* p1, const point2d_t* p2, const point2d_t* p3,
               const point2d_t* p4) {
  double area = geom_triangle_area(p1, p2, p3) + geom_triangle_area(p1, p3, p4);
  if (geom_segs_intersect(p1, p3, p2, p4)) {
    area /= 2;
  }
  return area;
}





char
geom_segs_intersect(const point2d_t* a1, const point2d_t* a2,
                    const point2d_t* b1, const point2d_t* b2) {

  // Method taken from:
  // http://stackoverflow.com/questions/563198/
  //              how-do-you-detect-where-two-line-segments-intersect

  point2d_t r = { a2->x - a1->x, a2->y - a1->y };
  point2d_t s = { b2->x - b1->x, b2->y - b1->y };

  // Some pre-computing.
  const double r_s = vec_cross_prod(&r, &s);   // r x s
  point2d_t q_p;
  vec_sub(&q_p, b1, a1);                         // q - p
  double q_p_r = vec_cross_prod(&q_p, &r);

  // Preliminary checks.
  if (GEOM_EQ_0(r_s)) {
    if (GEOM_EQ_0(q_p_r)) {
      // collinear
    } else {
      // parallel
    }
    return 0;
  }

  // Rest of the computation.
  double t = vec_cross_prod(&q_p, &s) / r_s;
  double u = q_p_r / r_s;

  // Remaining checks.
  if (GEOM_IN_R(t, 0, 1) && GEOM_IN_R(u, 0, 1)) {
    return 1;
  }

  // Intersects the line, not the segment.
  return 0;
}
