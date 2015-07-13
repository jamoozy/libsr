/*! @file point.h
 * Header file that defines the structures:
 *
 * 1. `point2d_t`
 * 2. `point2dt_t`
 * 3. `point_t`
 *
 * And several function-specific convenience functions.
 *
 * The structures defined here take advantage of unions and the way structures
 * are stored to disk with a little macro magic to create a kind of object
 * hierarchy.  All point `struct`s are easily cast-able to `super`-`structs` by
 * calling, e.g., `.p2d` on it.
 */

#ifndef __point_h__
#define __point_h__

#include <assert.h>
#include <stdlib.h>
#include <math.h>

/*! @struct point2d_t
 * A simple 2D point.
 *
 * @param long x The X-coordinate.
 * @param long y The Y-coordinate.
 */

/*! The body of a `point2d_t` */
#define POINT2D_BODY long x; long y;
/*! The actual `point2d_t` structure. */
#define POINT2D_STRUCT struct { POINT2D_BODY }
#ifndef SWIG
/*! Union for syntax sugar. (only available outside swig). */
#define POINT2D_UNION union { point2d_t p2d; POINT2D_STRUCT; };
#endif
typedef POINT2D_STRUCT point2d_t;

/*! @struct point2dt_t
 * A 2D point with timestamp;
 *
 * @param x The X-coordinate.
 * @param y The Y-coordinate.
 * @param t Unix time of creation (for drawn points)
 */
#ifdef SWIG
#define POINT2DT_BODY POINT2D_BODY long t;
#else
/*! The body of a `point2dt_t` */
#define POINT2DT_BODY POINT2D_UNION long t;
#endif
/*! The actual `point2dt_t` structure */
#define POINT2DT_STRUCT struct { POINT2DT_BODY }
#ifndef SWIG
/*! Union for syntax sugar. (only available outside swig). */
#define POINT2DT_UNION union { point2dt_t p2dt; POINT2DT_STRUCT; };
#endif
typedef POINT2DT_STRUCT point2dt_t;

/*! @struct point_t
 * A point structure with coordinates in space, time, and index.
 *
 * @param x The X-coordinate.
 * @param y The Y-coordinate.
 * @param t Unix time of creation (for drawn points)
 * @param i Index (for strokes)
 */
#ifdef SWIG
#define POINT_BODY POINT2DT_BODY long i;
#else
/*! The body of a `point_t` */
#define POINT_BODY POINT2DT_UNION long i;
#endif
/*! The actual `point_t` structure */
#define POINT_STRUCT struct { POINT_BODY }
#ifndef SWIG
/*! Union for syntax sugar. (only available outside swig). */
#define POINT_UNION union { point_t p; POINT_STRUCT; };
#endif
typedef POINT_STRUCT point_t;

/*! Creates a point.  The caller must call [@ref point_destroy(point_t*)] to
 * free the memory allocated by this function.
 *
 * @return a new `point_t`
 */
point_t* point_create();

/*! Creates a point at a specific place.  The caller must call
 * [@ref point_destroy(point_t*)] to free the memory allocated by this function.
 *
 * @param x The X coordinate.
 * @param y The Y coordinate.
 *
 * @return [@ref point_t]
 */
point_t* point_create_coords(long x, long y);

/*! Creates a timed point.  The caller must call [@ref point_destroy(point_t*)]
 * to free the memory allocated by this function.
 *
 * @param x The X coordinate.
 * @param y The Y coordinate.
 * @param t The unix time it was created.
 *
 * @return [@ref point_t]
 */
point_t* point_create_timed(long x, long y, long t);

/*! Creates a fully-populated point.  The caller must call
 * [@ref point_destroy(point_t*)] to free the memory allocated by this function.
 *
 * @param x The X coordinate.
 * @param y The Y coordinate.
 * @param t The unix time it was created.
 * @param i The index in the stroke.
 *
 * @return [@ref point_t]
 */
point_t* point_create_full(long x, long y, long t, long i);

/*! Destroys (frees the memory of) a point.
 *
 * @param self The point to destroy.
 */
void point_destroy(point_t* self);



////////////////////////////////////////////////////////////////////////////////
// ------------------------------ Point Utils ------------------------------- //
////////////////////////////////////////////////////////////////////////////////

/*!
 * Computes the distance from `a` to `b`.
 *
 * @param a A [@ref point_t].
 * @param b Another [@ref point_t].
 *
 * @return The distance between the points.
 */
static inline double point2d_distance(const point2d_t* a, const point2d_t* b) {
  const long diff_x = a->x - b->x;
  const long diff_y = a->y - b->y;
  return sqrt(diff_x * diff_x + diff_y * diff_y);
}

/*! Computes the angle between two lines, both with an endpoint at the origin
 * (`(0, 0`) and the other at `a` and `b` respectively.
 *
 * @param a The endpoint of one line.
 * @param a The endpoint of the other line.
 *
 * @return The angle between the lines.
 */
static inline double point2d_angle_to(const point2d_t* a, const point2d_t* b) {
  return atan2(b->y - a->y, b->x - a->x);
}

/*! Accumulates the value of b into a.
 *
 * @param a The destination point.
 * @param b The source point.
 */
static inline void point2d_accum(point2d_t* a, const point2d_t* b) {
  a->x += b->x;
  a->y += b->y;
}

/*! Divides a [@ref point2d_t] by a scalar.
 *
 * @param a A point.
 * @param s A scalar to divide by.
 */
static inline void point2d_div(point2d_t* a, double s) {
  a->x /= s;
  a->y /= s;
}

/*! Finds the point exactly between `a` and `b`.
 *
 * @param a A [@ref point2d_t]
 * @param b Another [@ref point2d_t]
 *
 * @return The point directly between `a` and `b`.
 */
static inline void point2d_center(point2d_t* out,
    const point2d_t* a, const point2d_t* b) {
  assert(a->x != b->x || a->y != b->y);
  out->x = (a->x + b->x) / 2;
  out->y = (a->y + b->y) / 2;
}

/*! Computes 2 points on the perpendicular bisector through the midpoint of the
 * line segment defined by `a` and `b`.
 *
 * @param o1 One output point.
 * @param l2 Another output point.
 * @param a An input point.
 * @param b Another input point.
 */
void point2d_bis(point2d_t* o1, point2d_t* o2,
    const point2d_t* a, const point2d_t* b);


#endif // __point_h__
