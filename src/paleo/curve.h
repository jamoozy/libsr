/*!
 * \addtogroup pal PaleoSketch
 * \{
 *
 * \file curve.h
 * Defines the interface for the curve test submodule.
 */

#ifndef __pal_curve_h__
#define __pal_curve_h__

#include "common/point.h"

#include "paleo.h"
#include "curve.h"


#define CURVE_CONTROL_POINT_CAP 5

//! A Bezier curve with some number of control points.
typedef struct {
  point2d_t pts[CURVE_CONTROL_POINT_CAP];   //!< The control points.
  int num;                                  //!< The number of control points
} pal_curve_t;

//! The recognition result for the curve test.
typedef struct {
  PAL_RESULT_UNION;
  pal_curve_t curve;    //!< The built curve.
} pal_curve_result_t;

//! The test context for curves.
typedef struct {
  const pal_stroke_t* stroke;  //!< The stroke to test.
  double* Xs;           //!< The X coordinates in the stroke.
  double* Ys;           //!< The Y coordinates in the stroke.

  //! The ideal Bezier curve with 4 points.
  struct {
    point2d_t Cs[4];    //!< Computed control points
    double lse;         //!< Least Squares Error
  } ideal_4;

  //! The ideal Bezier curve with 5 points.
  struct {
    point2d_t Cs[5];    //!< Computed control points
    double lse;         //!< Least Squares Error
  } ideal_5;
  pal_curve_result_t result;   //!< Both results, 4 then 5.
} pal_curve_context_t;



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Functions -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

/*!
 * Creates a new curve at with the specified number of points, all of which are
 * at \f$(0,0)\f$.
 *
 * \param num The number of points in the curve.
 *
 * \return The new curve.
 */
pal_curve_t* pal_curve_create(long num);

/*!
 * Creates a curve with newly created points at the specified coordinates.
 *
 * \param num The number of points in the curve.
 * \param xs X coordinates.
 * \param ys Y coordinates.
 *
 * \return The new curve.
 */
pal_curve_t* pal_curve_create_coords(long num, long* xs, long* ys);

/*!
 * Creates a curve with the given points.
 *
 * \param num The number of points.
 * \param points The points.
 *
 * \return The new curve.
 */
pal_curve_t* pal_curve_create_points(long num, const point2d_t* points);

/*!
 * Computes \c num evenly separated points the Bézier curve.
 *
 * \param self The curve.
 * \param out The output points.
 * \param num The number of points to generate.
 */
void pal_curve_compute_points(pal_curve_t* self, point2d_t* out, int num);

/*!
 * Does a deep copy of an curve.
 *
 * \param dst The destination curve .
 * \param src The source curve.
 */
#define pal_curve_cpy(dst,src) memcpy(dst, src, sizeof(pal_curve_t));

/*!
 * Destroys the curve by freeing its memory.
 *
 * \param self The curve to free.
 */
static inline void pal_curve_destroy(pal_curve_t* self) { free(self); }

/*!
 * Recursively computes a point on a Bézier curve.
 *
 * \param p The return point.
 * \param pts The control points.
 * \param num_pts The number of control points.
 * \param t The parameter to the Bézier function.
 */
void pal_curve_util_compute_point(point2d_t *p,
    const point2d_t* pts, int num_pts, double t);



/*! Initialize the curve test. */
void pal_curve_init();

/*! De-initializes the curve test by freeing its memory. */
void pal_curve_deinit();

/*!
 * Does the curve test on the Paleo stroke.
 *
 * \param stroke The stroke to test.
 *
 * \return The result.
 */
const pal_curve_result_t* pal_curve_test(const pal_stroke_t* stroke);

/*!
 * Does a deep copy of a curve.
 *
 * \param dst The destination curve.
 * \param src The source curve.
 */
static inline void
pal_curve_result_cpy(pal_curve_result_t* dst, const pal_curve_result_t* src) {
  memcpy(dst, src, sizeof(pal_curve_t));
}

/*!
 * Clones a curve result.  The caller is responsible for freeing the returned
 * clone.
 *
 * \param self The curve result to clone.
 *
 * \return The cloned result.
 */
static inline pal_curve_result_t*
pal_curve_result_cln(const pal_curve_result_t* self) {
  pal_curve_result_t* clone = malloc(sizeof(pal_curve_result_t));
  pal_curve_result_cpy(clone, self);
  return clone;
}

/*!
 * Destroys the result and frees its memory.
 *
 * \param self The result to free.
 */
static inline void pal_curve_result_destroy(pal_curve_result_t* self) {
  free(self);
}

#endif // __pal_curve_h__

/*! \} */
