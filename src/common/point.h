#ifndef __point_h__
#define __point_h__


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

double inline point2d_distance(const point2d_t* a, const point2d_t* b) {
  const long diff_x = a->x - b->x;
  const long diff_y = a->y - b->y;
  return sqrt(diff_x * diff_x + diff_y * diff_y);
}

#endif // __point_h__

