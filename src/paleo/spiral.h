#ifndef __spiral_h__
#define __spiral_h__

#include "common/point.h"

typedef struct {
  point2d_t center;
  double r;
  double theta_f;
  double theta_t;
  int cw;
} spiral_t;

// Creates a new spiral.
spiral_t* spiral_create();

// Destroys the spiral by freeing its memory.
void spiral_destroy(spiral_t*);

// Computes 'n' evenly distributed points in the spiral and returns them in the
// 'pts' array.
void spiral_points(const spiral_t* self, point2d_t *pts, int n);

// Computes a single point t% of the way through the spiral.
void spiral_point_at(const spiral_t* self, point2d_t* p, double t);

#endif // __spiral_h__
