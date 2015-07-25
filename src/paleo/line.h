/*! \file line.h
 * Defines the interface to the line recognition submodule of PaleoSketch.
 *
 * \addtogroup pal
 * \{
 */

#ifndef __pal_line_h__
#define __pal_line_h__

#include "common/point.h"

#include "paleo.h"
#include "line.h"

//! A line with some number of joints.
typedef struct {
  size_t num;      //!< The number of points, including start and end.
  point2d_t* pts;  //!< The points.  Must be at least 2.
} pal_line_t;

/*! The sub-results of the line result.  The 0<sup>th</sup> will be the result
 * of the actual line/polyline; the i<sup>th</sup> will be the result of the
 * i<sup>th</sup> sub-line (for plines, only).
 */
typedef struct {
  PAL_RESULT_UNION;
  pal_line_t line;   //!< The recognized line.
} pal_line_sub_result_t;

//! The results specific to a line test.
typedef struct {
  pal_line_sub_result_t *res;   //!< The results.
  int num;                      //!< The number of results.
} pal_line_result_t;

//! Context needed to perform the line test.
typedef struct {
  const pal_stroke_t* stroke;   //!< The stroke to recognize.

  //! The ideal line.
  struct {
    double slope;  //!< slope
    double y_int;  //!< Y-intercept
    point2d_t p0;  //!< First point in the ideal line.
    double theta;  //!< Angle of the line (from p0).
  } ideal;

  /*! Result of all line tests performed by the last call to pal_line_test or
   * pal_pline_test.  For the simple line test there will be only one result
   * here -- the result of the single line test performed.  For the poly-line,
   * there will be num_crnrs tests here.  The 0th will be the final poly-line
   * test, and the nth will be the line segment test for the nth subsegment of
   * the line.
   */
  pal_line_result_t res;
} pal_line_context_t;





/*! Creates a new line from \f$(0,0)\f$ to \f$(0,0)\f$. */
pal_line_t* pal_line_create();

/*! Creates a line with the given points.
 *
 * \param a The starting point of the line.
 * \param b The ending point of the line.
 *
 * \return The new line.
 */
pal_line_t* pal_line_create_points_with_points(
    const point2d_t* a, const point2d_t* b);

/*! Creates a line containing newly created points with the given coordinates.
 *
 * \param ax The starting point's X coord.
 * \param ay The starting point's Y coord.
 * \param bx The ending point's X coord.
 * \param by The ending point's Y coord.
 *
 * \return The new line.
 */
pal_line_t* pal_line_create_points_with_longs(
    long ax, long ay, long bx, long by);

/*! Does a deep copy of a line.
 *
 * \param dst The destination line.
 * \param src The source line.
 */
static inline void pal_line_cpy(pal_line_t* dst, const pal_line_t* src) {
  dst->pts = malloc((dst->num = src->num) * sizeof(point2d_t));
  memcpy(dst->pts, src->pts, src->num * sizeof(point2d_t));
}

/*! Returns the rank of the line.
 *
 * \param self The line.
 *
 * \return The rank.
 */
static inline int pal_line_rank(const pal_line_t* self) {
  return self->num-1;
}

/*! Frees the memory in a line.
 *
 * \param self The line to free.
 */
static inline void pal_line_destroy(pal_line_t* self) {
  free(self->pts);
  free(self);
}





/*! Initialize the line test. */
void pal_line_init();

/*! De-initializes the line test by freeing its memory. */
void pal_line_deinit();

/* Does a deep copy of a line result.
 *
 * \param dst The destination line result.
 * \param src The source line result.
 */
static inline void
pal_line_result_cpy(pal_line_result_t* dst, const pal_line_result_t* src) {
  dst->num = src->num;
  for (int i = 0; i < src->num; i++) {
    memcpy(&dst->res[i], &src->res[i], sizeof(pal_line_sub_result_t));
    pal_line_cpy(&dst->res[i].line, &src->res[i].line);
  }
}

/*! Does the line test on the paleo stroke.
 *
 * \param stroke The stroke to test.
 *
 * \return The result of the test.
 */
const pal_line_result_t* pal_line_test(const pal_stroke_t* stroke);

/*! Does the poly line test on the paleo stroke.
 *
 * \param stroke The stroke to test.
 *
 * \return The result of the test.
 */
const pal_line_result_t* pal_pline_test(const pal_stroke_t* stroke);


#endif // __pal_line_h__

/*! \} */
