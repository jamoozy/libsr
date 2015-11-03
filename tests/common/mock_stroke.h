#ifndef __mock_stroke_h__
#define __mock_stroke_h__

#include "common/stroke.h"

/*! Creates a mock stroke containing new points with the passed values.
 *
 * This is a `va_arg` function that expects `num` triplets of `double`,
 * `double`, `long` that correspond to the x, y, and t values of `num` points.
 *
 * \param num The number of points.
 * \param ... The point data.
 *
 * \return The stroke.  Must be freed with call to `stroke_destroy`.
 */
stroke_t* mock_stroke(int num, ...);

#endif // __mock_stroke_h__
