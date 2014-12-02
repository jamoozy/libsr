#ifndef __ellipse_h__
#define __ellipse_h__

#include "common/point.h"

// A ellipse with some number of joints.
typedef struct {
  long r;       // The radius of the ellipse.
  point2d_t c;  // The center of the ellipse.
} ellipse_t;

// Creates a new ellipse at 0,0 with radius 0.
ellipse_t* ellipse_create();

// Creates a ellipse with the given center and radius.
//   r: The radius.
//   x: X coordinate.
//   y: Y coordinate.
ellipse_t* ellipse_create_full(long r, long x, long y);

// Creates a ellipse with the given center and radius.  The passed center will
// be taken over by the ellipse -- in other words, it will not be cloned in
// this function; it will be freed on a call to ellipse_destroy(ellipse_t*).
//   r: The radius.
//   c: The center.
ellipse_t* ellipse_create_with_point(long r, const point2d_t* c);

// Destroys the ellipse by freeing its memory.
void ellipse_destroy(ellipse_t*);

#endif // __ellipse_h__
