/*! @file geom.h
 * Several generally useful geometry functions.  See also @ref point.h and @ref
 * stroke.h for stroke- and point-specific functions.
 */

#ifndef __common_geom_h__
#define __common_geom_h__

#include <math.h>

#include "util.h"
#include "point.h"

//! Amount to allow for number rounding errors.
#define GEOM_ERR 0.00000001

//! Determines if the value is low enough to be considered 0.
#define GEOM_EQ_0(x) (abs(x) <= GEOM_ERR)

//! Returns whether \f$x\in[a,z]\f$ subject to `GEOM_ERR`.
#define GEOM_IN_R(x,a,z) ((a) - GEOM_ERR < (x) && (x) < (z) + GEOM_ERR)

/*! Finds the cross-product of the two lines.
 *
 * @param u A "vector" (from the origin).
 * @param v Another "vector" (from the origin).
 *
 * @return The cross product.
 */
static inline double vec_cross_prod(const point2d_t* u, const point2d_t* v) {
  return u->x * v->y - v->x * u->y;
}

/*! Computes cross product: \f$u \times v\f$ and stores into `rtn`.
 *
 * @param rtn "Return value"
 * @param u A "vector" (from the origin).
 * @param v Another "vector" (from the origin).
 */
static inline void
vec_sub(point2d_t* rtn, const point2d_t* u, const point2d_t* v) {
  rtn->x = u->x - v->x;
  rtn->y = u->y - v->y;
}



/*! Determines if the line `a1`\f$\rightarrow\f$`a2` intersects the line
 * `b1`\f$\rightarrow\f$`b2`.
 *
 * @param a1 Line #1's 1st point.
 * @param a2 Line #1's 2nd point.
 * @param b1 Line #2's 1st point.
 * @param b2 Line #2's 2nd point.
 *
 * @return 1 if the lines intersect, 0 otherwise.
 */
char geom_seg_seg_intersect(const point2d_t* a1, const point2d_t* a2,
                            const point2d_t* b1, const point2d_t* b2);

/*! Determines if the line segment `s1`\f$\rightarrow\f$`s2` intersects the
 * infinite line defined by `l1`\f$\rightarrow\f$`l2`.
 *
 * @param s1 First point in line segment.
 * @param s2 Second point in line segment.
 * @param l1 A point in the (infinitely long) line.
 * @param l2 A point in the (infinitely long) line.
 *
 * @return 1 if they intersect, 0 otherwise.
 */
char geom_seg_line_intersect(const point2d_t* s1, const point2d_t* s2,
                             const point2d_t* l1, const point2d_t* l2);

/*! Computes the intersection point between the line segment defined by
 * `s1`\f$\rightarrow\f$`s2` and the infinite line defined by
 * `l1`\f$\rightarrow\f$`l2`.
 *
 * @param isect Return intersection point.
 * @param s1 First point in line segment.
 * @param s2 Second point in line segment.
 * @param l1 A point in the (infinitely long) line.
 * @param l2 A point in the (infinitely long) line.
 *
 * @return 1 on success and 0 on failure.
 */
char geom_seg_line_intersection(point2d_t* isect,
    const point2d_t* s1, const point2d_t* s2,
    const point2d_t* l1, const point2d_t* l2);

/*! Finds the intersection between 2 infinitely long lines.
 *
 * @param isect "Return" intersection point.
 * @param s1 First point in first (infinitely long) line.
 * @param s2 Second point in first (infinitely long) line.
 * @param l1 A point in the (infinitely long) line.
 * @param l2 A point in the (infinitely long) line.
 *
 * @return 1 on success and 0 on failure.
 */
char geom_line_line_intersection(point2d_t* isect,
    const point2d_t* s1, const point2d_t* s2,
    const point2d_t* l1, const point2d_t* l2);



/*! Computes the area of the triangle defined by the 3 passed points.
 *
 * @param p1 A point that defines the triangle.
 * @param p2 A point that defines the triangle.
 * @param p3 A point that defines the triangle.
 *
 * @return The area of the triangle.
 */
double geom_triangle_area(const point2d_t* p1, const point2d_t* p2,
                          const point2d_t* p3);

/*! Computes the area of an arbitrary quadrilateral defined by `a`, `b`, `c`,
 * `d`.  The order of the point determines the shape of the quadrilateral -- `a`
 * is connected to `b` and `d`, `b` is connected to `a` and `c`, and so on.
 * This *should* be able to handle "folded" quads.
 *
 * @param p1 A point that defines the quadrilateral.
 * @param p2 A point that defines the quadrilateral.
 * @param p3 A point that defines the quadrilateral.
 * @param p4 A point that defines the quadrilateral.
 *
 * @return The area of the quadrilateral.
 */
double geom_quad_area(const point2d_t* p1, const point2d_t* p2,
                      const point2d_t* p3, const point2d_t* p4);


/*! Computes two points defining the line orthogonal to the input points through
 * the center point.
 *
 * @param out1 Output point #1.
 * @param out2 Output point #2.
 * @param in1 Input point #1.
 * @param in2 Input point #2.
 * @param c Center point to create output line through.
 */
static inline void geom_ortho_line(point2d_t* out1, point2d_t* out2,
    const point2d_t* in1, const point2d_t* in2, const point2d_t* c) {
  out1->x = in1->y - c->y + c->x;
  out2->y = in2->x - c->x + c->y;
}



/*! Computes the distance from a point, `p`, to the closest point on a line
 * defined by `a` and `b`.
 *
 * @param p The point.
 * @param a A point on the line.
 * @param b Another point on the line.
 *
 * @return The distance from the point to the line.
 */
static inline double geom_point_dist_to_line(const point2d_t* p,
    const point2d_t* a, const point2d_t* b) {
  double angle = point2d_angle_to(a, p) - point2d_angle_to(a, b);
  return abs(point2d_distance(a, p) * sin(angle));
}

#endif  // __common_geom_h__
