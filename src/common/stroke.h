#ifndef __stroke_h__
#define __stroke_h__

#include <stdlib.h>
#include "point.h"

typedef struct {
  long num;      // Number of points in the points array.
  long size;     // Used size of points.
  point_t* pts;  // The points.
} stroke_t;

// Creates a stroke with no points and the given size.
//   size: The size of the points array.
stroke_t* stroke_create(int size);

// Creates a stroke with the given points.  DOES NOT take over the points; they
// must be destroyed by the caller.
//   size: The number of points in points
//   points: The points.
stroke_t* stroke_create_point2dts(int size, const point2dt_t* pts);

// Adds a point to the stroke.  Increases the size if necessary.  DOES NOT take
// over the points; they must be destroyed by the caller.
//   self: The stroke to add to.
//   point: The point to add.
void stroke_add_point2dt(stroke_t* self, const point2dt_t* point);

// Creates a point at the coordinates and adds it to the stroke.  Uses current
// time as time of creation.
//   self: The stroke to add to.
//   x: The x coordinate.
//   y: The y coordinate.
void stroke_add_coords(stroke_t* self, long x, long y);

// Creates a point at the coordinates and adds it to the stroke.  Uses current
// time as time of creation.
//   self: The stroke to add to.
//   x: The x coordinate.
//   y: The y coordinate.
void stroke_add_timed(stroke_t* self, long x, long y, long t);

// Destroys the stroke and all its contained points by freeing their associated
// memory.
//   self: The stroke to destroy.
static inline void stroke_destroy(stroke_t* self) {
  free(self->pts);
  free(self);
}



////////////////////////////////////////////////////////////////////////////////
//                                  File I/O                                  //
////////////////////////////////////////////////////////////////////////////////

// Saves a stroke to disk.
//   self: The stroke to save.
//   fname: The file to save it to.
// Returns: 1 on success, 0 o.w.
int stroke_save(stroke_t* self, const char* fname);

// Loads a stroke from disk.
//   fname: The name of the file to load.
stroke_t* stroke_from_file(const char* fname);



////////////////////////////////////////////////////////////////////////////////
//                          SWIG-specific functions                           //
////////////////////////////////////////////////////////////////////////////////

// Gets a pointer to an existing point in the stroke.  Do not free the memory
// address passed!  It will result in memory corruption!
//   self: The stroke.
//   i: The index of the point to get.
point_t* stroke_get(const stroke_t* self, int i);

#endif  // __stroke_h__
