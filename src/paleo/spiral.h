#ifndef __pal_spiral_h__
#define __pal_spiral_h__

#include "common/point.h"

#include "paleo.h"

typedef struct {
  point2d_t center;
  double r;
  double theta_t;
  double theta_f;
  int cw;
} pal_spiral_t;

typedef struct {
  PAL_TEST_RESULT_UNION;
  pal_spiral_t spiral;
} pal_spiral_result_t;

typedef struct {
  const pal_stroke_t* stroke;
  struct {
    point2d_t center;
    double r;
  } ideal;
  pal_spiral_result_t result;
} pal_spiral_context_t;



// Creates a new spiral.
pal_spiral_t* pal_spiral_create();

// Destroys the spiral by freeing its memory.
void pal_spiral_destroy(pal_spiral_t* self);

// Computes 'n' evenly distributed points in the spiral and returns them in the
// 'pts' array.
void pal_spiral_points(const pal_spiral_t* self, point2d_t *pts, int n);

// Computes a single point t% of the way through the spiral.
void pal_spiral_point_at(const pal_spiral_t* self, point2d_t* p, double t);

#endif // __pal_spiral_h__
