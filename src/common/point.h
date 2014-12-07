#ifndef __point_h__
#define __point_h__

#include <assert.h>
#include <stdlib.h>
#include <math.h>

// A simple 2D point.
//   x: The X-coordinate.
//   y: The Y-coordinate.
#define POINT2D_BODY long x; long y;
#define POINT2D_STRUCT struct { POINT2D_BODY }
#define POINT2D_UNION union { point2d_t p2d; POINT2D_STRUCT; };
typedef POINT2D_STRUCT point2d_t;

// A 2D point with timestamp;
//   x: The X-coordinate.
//   y: The Y-coordinate.
//   t: Unix time of creation (for drawn points)
#define POINT2DT_BODY POINT2D_UNION long t;
#define POINT2DT_STRUCT struct { POINT2DT_BODY }
#define POINT2DT_UNION union { point2dt_t p2dt; POINT2DT_STRUCT; };
typedef POINT2DT_STRUCT point2dt_t;

// A point structure with coordinates in space, time, and index.
//   x: The X-coordinate.
//   y: The Y-coordinate.
//   t: Unix time of creation (for drawn points)
//   i: Index (for strokes)
#define POINT_BODY POINT2DT_UNION long i;
#define POINT_STRUCT struct { POINT_BODY }
#define POINT_UNION union { point_t p; POINT_STRUCT; };
typedef POINT_STRUCT point_t;

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
void point_destroy(point_t* self);



////////////////////////////////////////////////////////////////////////////////
// ------------------------------ Point Utils ------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Computes the distance from a to b.
static inline double point2d_distance(const point2d_t* a, const point2d_t* b) {
  const long diff_x = a->x - b->x;
  const long diff_y = a->y - b->y;
  return sqrt(diff_x * diff_x + diff_y * diff_y);
}

// Computes the angle from a to b.
static inline double point2d_angle_to(const point2d_t* a, const point2d_t* b) {
  return atan2(b->y - a->y, b->x - a->x);
}

// Accumulates the value of b into a.
static inline void point2d_accum(point2d_t* a, const point2d_t* b) {
  a->x += b->x;
  a->y += b->y;
}

// Divides a point2d_t by a scalar.
static inline void point2d_div(point2d_t* a, double s) {
  a->x /= s;
  a->y /= s;
}

// Finds the point between a and b.
static inline void point2d_center(point2d_t* out,
    const point2d_t* a, const point2d_t* b) {
  assert(a->x != b->x || a->y != b->y);
  out->x = (a->x + b->x) / 2;
  out->y = (a->y + b->y) / 2;
}

// Computes 2 points on the perpendicular bisector through the midpoint of the
// line segment defined by a and b.
void point2d_bis(point2d_t* o1, point2d_t* o2,
    const point2d_t* a, const point2d_t* b);


#endif // __point_h__
