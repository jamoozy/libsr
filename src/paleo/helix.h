#ifndef __paleo_helix_h__
#define __paleo_helix_h__

#include "common/point.h"

typedef struct {
  point2d_t c[2];   // Starting and ending center points.
  double r;         // Radius.
  double theta_i;   // Initial angle.
  double theta_t;   // Angle traversed.
  int cw;           // Clockwise?
} helix_t;

// Creates a new helix.
helix_t* helix_create();

// Destroys the helix by freeing its memory.
void helix_destroy(helix_t*);

// Computes 'num' evenly spaced points.
void helix_compute_points(const helix_t* self, point2d_t* p, int num);

// Computes a single point at the parametric value 't'.
void helix_compute_point(const helix_t* self, point2d_t* p, double t);

#endif // __paleo_helix_h__
