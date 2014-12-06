#ifndef __ellipse_h__
#define __ellipse_h__

#include "common/point.h"

// Much of this math is based on what I learned from:
//    http://mathworld.wolfram.com/Ellipse.html

// A ellipse with some number of joints.
typedef struct {
  point2d_t f1;  // The center of the ellipse.
  point2d_t f2;  // The center of the ellipse.
  double maj;    // Major axis length.
  double min;    // Minor axis length.
} ellipse_t;

// Creates a new ellipse with the given focci and maj- and min-length
ellipse_t* ellipse_create(
    const point2d_t* f1, const point2d_t* f2, double maj, double min);

// Populate the already-allocated ellipse.
void ellipse_populate(ellipse_t* self,
    const point2d_t* f1, const point2d_t* f2, double maj, double min);

// Destroys the ellipse by freeing its memory.
void ellipse_destroy(ellipse_t* self);

#endif // __ellipse_h__
