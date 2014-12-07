#ifndef __arc_h__
#define __arc_h__

#include "common/point.h"

// A arc with some number of joints.
typedef struct {
  point2d_t endpts[2];    // Endpoints of the arc.
  point2d_t center;       // The center of the arc.
  char cw;                // Clockwise?
} arc_t;

// Creates a new arc at 0,0 with radius 0.
arc_t* arc_create(const point2d_t* e1, const point2d_t* e2, const point2d_t* c,
    char cw);

// Destroys the arc by freeing its memory.
void arc_destroy(arc_t*);

#endif // __arc_h__
