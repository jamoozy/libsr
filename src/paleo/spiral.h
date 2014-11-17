#ifndef __spiral_h__
#define __spiral_h__

#include "common/point.h"

typedef struct {
} spiral_t;

// Creates a new spiral.
spiral_t* spiral_create();

// Destroys the spiral by freeing its memory.
void spiral_destroy(spiral_t*);

#endif // __spiral_h__
