/*!
 * \addtogroup common
 * \{
 *
 * \file stroke.h
 * Creates an object-like `stroke_t` structure.
 *
 * For file I/O, see `stroke_save()` and `stroke_from_file()`.
 */

#ifndef __stroke_h__
#define __stroke_h__

#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "point.h"
#include "debug.h"

/*!
 * A stroke object; basically an array of points.  This represents how the
 * user's pen moved during (and including) mouse up and mouse down events.
 */
typedef struct {
  long num;      //!< Number of points in the points array.
  long size;     //!< Capacity of `pts` array.
  point_t* pts;  //!< The points.
} stroke_t;

/*!
 * Creates a stroke with no points and the given size.
 *
 * \param size The size of the points array.
 *
 * \return The (empty) stroke.
 */
stroke_t* stroke_create(int size);

/*!
 * Creates a stroke with the given points.  **Does not** take over the points;
 * it copies them.  As a result, the passed points must be destroyed by the
 * caller.
 *
 * \param size The number of points in points
 * \param pts The points.
 *
 * \return a new stroke object.
 */
stroke_t* stroke_create_point2dts(int size, const point2dt_t* pts);

/*!
 * Adds a point to the stroke.  Increases the size if necessary.  **Does not**
 * take over the points; it copies them.  As a result, the passed points must be
 * destroyed by the caller.
 *
 * \param self The stroke to add to.
 * \param point The point to add.
 */
void stroke_add_point2dt(stroke_t* self, const point2dt_t* point);

/*!
 * Creates a point at the coordinates and adds it to the stroke.  Uses current
 * time as time of creation.
 *
 * \param self The stroke to add to.
 * \param x The x coordinate.
 * \param y The y coordinate.
 */
void stroke_add_coords(stroke_t* self, long x, long y);

/*!
 * Creates a point at the coordinates and adds it to the stroke.  Uses current
 * time as time of creation.
 *
 * \param self The stroke to add to.
 * \param x The x coordinate.
 * \param y The y coordinate.
 * \param t The unix time the point was made.
 */
void stroke_add_timed(stroke_t* self, long x, long y, long t);

/*! Inserts a point at a specific index in a stroke.
 *
 * \param self The stroke.
 * \param i The index to insert at.
 * \param x The X-coordinate.
 * \param y The Y-coordinate.
 */
void stroke_insert_at(stroke_t* self, int i, long x, long y);

/*!
 * Destroys the stroke and all its contained points by freeing their associated
 * memory.
 *
 * \param self The stroke to destroy.
 */
static inline void stroke_destroy(stroke_t* self) {
  debug("Freeing self->pts: %p ...\n", self->pts);
  free(self->pts);
  bzero(self, sizeof(stroke_t));

  debug("Freeing self: %p ...\n", self);
  free(self);
  self = NULL;
}

/*!
 * Performs a deep copy of the stroke and returns it.
 *
 * \param strk The stroke to clone.
 *
 * \return The clone.
 */
static inline stroke_t* stroke_clone(const stroke_t* strk) {
  stroke_t* clone = malloc(sizeof(stroke_t));
  memcpy(clone, strk, sizeof(stroke_t));
  clone->pts = calloc(strk->size, sizeof(point_t));
  memcpy(clone->pts, strk->pts, strk->size * sizeof(strk->pts[0]));
  return clone;
}



////////////////////////////////////////////////////////////////////////////////
//                                  File I/O                                  //
////////////////////////////////////////////////////////////////////////////////

/*!
 * Saves a stroke to disk.
 *
 * \param self The stroke to save.
 * \param fname The file to save it to.
 *
 * \return 1 on success, 0 o.w.
 */
int stroke_save(stroke_t* self, const char* fname);

/*!
 * Loads a stroke from disk.
 *
 * \param fname The name of the file to load.
 *
 * \return The stroke that was in the file.
 */
stroke_t* stroke_from_file(const char* fname);



////////////////////////////////////////////////////////////////////////////////
//                          SWIG-specific functions                           //
////////////////////////////////////////////////////////////////////////////////

/*!
 * Gets a pointer to an existing point in the stroke.  **Do not** free the
 * memory address passed!  It will result in memory corruption!
 *
 * \param self The stroke.
 * \param i The index of the point to get.
 *
 * \return A reference to the stroke's internal point.
 */
point_t* stroke_get(const stroke_t* self, int i);

#endif  // __stroke_h__

/*! \} */
