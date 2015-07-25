/*! \file spiral.h
 * Defines the interface to the portion of PaleoSketch that recognizes spirals.
 *
 * \addtogroup
 * \{
 */

#ifndef __pal_spiral_h__
#define __pal_spiral_h__

#include "common/point.h"

#include "paleo.h"

//! The spiral shape.
typedef struct {
  point2d_t center;   //!< The center of the spiral.
  double r;           //!< The maximum radius of the spiral.
  double theta_t;     //!< The total angle traversed.
  double theta_f;     //!< The final angle to stop drawing on.
  int cw;             //!< Whether this is going clockwise.
} pal_spiral_t;

//! The spiral result.
typedef struct {
  PAL_RESULT_UNION;
  pal_spiral_t spiral;    //!< The shape.
} pal_spiral_result_t;

//! The spiral test context.
typedef struct {
  const pal_stroke_t* stroke;   //!< The stroke to recognize.

  //! The ideal spiral.
  struct {
    point2d_t center;           //!< The center of the spiral.
    double r;                   //!< The radius of the spiral.
  } ideal;
  pal_spiral_result_t result;   //!< The result to return.
} pal_spiral_context_t;



/*! Creates a new spiral. */
pal_spiral_t* pal_spiral_create();

/*! Destroys the spiral by freeing its memory.
 *
 * \param self The spiral to destroy.
 */
void pal_spiral_destroy(pal_spiral_t* self);

/* Does a deep copy of an spiral.
 *
 * \param dst The destination spiral.
 * \param src The source spiral.
 */
#define pal_spiral_cpy(dst,src) memcpy(dst, src, sizeof(pal_spiral_t));

/*! Draws \c n evenly distributed points in the spiral and returns them in the
 * \c pts array.
 *
 * \param self The spiral to create points for.
 * \param pts The points.
 * \param n The number of points to create.
 */
void pal_spiral_points(const pal_spiral_t* self, point2d_t *pts, int n);

/* Computes a single point that is \c t through the spiral.
 *
 * \param self The spiral to draw.
 * \param p The return point to compute.
 * \param t \f$t\in[0,1]\f$, denotes distance through spiral to compute.
 */
void pal_spiral_point_at(const pal_spiral_t* self, point2d_t* p, double t);



/*! Initialize the curve test. */
void pal_spiral_init();

/*! De-initializes the curve test. */
void pal_spiral_deinit();

/* Does the spiral test on the Paleo stroke.
 *
 * \param stroke The stroke to test.
 *
 * \return The result of the recognition.
 */
const pal_spiral_result_t* pal_spiral_test(const pal_stroke_t* stroke);

/* Does a deep copy of a curve result.
 *
 * \param dst The destination curve result.
 * \param src The source curve result.
 */
static inline void pal_spiral_result_cpy(
    pal_spiral_result_t* dst, const pal_spiral_result_t* src) {
  memcpy(dst, src, sizeof(pal_spiral_result_t));
}

#endif // __pal_spiral_h__

/*! \} */
