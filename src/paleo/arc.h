#ifndef __arc_h__
#define __arc_h__

#include "common/point.h"

// A arc with some number of joints.
typedef struct {
  point2d_t pts[2];   // Endpoints of the arc.
  point2d_t center;   // The center of the arc.
  double angle;       // Angle traversed from pts[0] to pts[1].
                      // (implies whether cw or ccw)
} arc_t;

// Creates a new arc with the given
arc_t* arc_create(const point2d_t* p1, const point2d_t* p2,
    const point2d_t* c, double angle);

// Populates a pre-allocated arc.
void arc_populate(arc_t* self, const point2d_t* p1, const point2d_t* p2,
    const point2d_t* c, double angle);

// Destroys the arc by freeing its memory.
void arc_destroy(arc_t*);

#endif // __arc_h__
