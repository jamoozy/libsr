/*!
 * \addtogroup pal
 * \{
 *
 * \file helix.h
 * Defines the interface for the PaleoSketch helix recognizer.
 */

#ifndef __pal_helix_h__
#define __pal_helix_h__

#include "common/point.h"

#include "paleo.h"

//! The helix shape.
typedef struct {
  point2d_t c[2];   //!< Starting and ending center points.
  double r;         //!< Radius.
  double theta_i;   //!< Initial angle.
  double theta_t;   //!< Angle traversed.
  int cw;           //!< Clockwise?
} pal_helix_t;

//! The helix test result.
typedef struct {
  PAL_RESULT_UNION;
  pal_helix_t helix;  //!< The helix we recognized.
} pal_helix_result_t;

//! The helix test context.
typedef struct {
  const pal_stroke_t* stroke;
  pal_helix_result_t result;  //!< The result.
} pal_helix_context_t;



//! Creates a new helix.
pal_helix_t* helix_create();

/*!
 * Destroys the helix by freeing its memory.
 *
 * \param self The helix to destroy.
 */
void pal_helix_destroy(pal_helix_t* self);

/*!
 * Does a deep copy of an helix.
 *
 * \param dst The destination helix.
 * \param src The source helix.
 */
#define pal_helix_cpy(dst,src) memcpy(dst, src, sizeof(pal_helix_t));

/*!
 * Computes `num` evenly spaced points.
 *
 * \param self The helix.
 * \param p The point.
 * \param num The number of points to create.
 */
void pal_helix_compute_points(const pal_helix_t* self, point2d_t* p, int num);

/*! Computes a single point at the parametric value `t`.
 *
 * \param self The helix.
 * \param p The return point.
 * \param t The parametric value.
 */
void pal_helix_compute_point(const pal_helix_t* self, point2d_t* p, double t);



/*! Initialize the curve test. */
void pal_helix_init();

/*! De-initializes the curve test. */
void pal_helix_deinit();

/*!
 * Test whether this is a helix.
 *
 * \param stroke The stroke to test.
 *
 * \return The result of the test.
 */
const pal_helix_result_t* pal_helix_test(const pal_stroke_t* stroke);

/*!
 * Does a deep copy of a helix result.
 *
 * \param dst The destination helix result.
 * \param src The source helix result.
 */
static inline void
pal_helix_result_cpy(pal_helix_result_t* dst, const pal_helix_result_t* src) {
  memcpy(dst, src, sizeof(pal_helix_result_t));
}

/*!
 * Clones the result.  The caller is responsible for freeing the returned
 * memory.
 *
 * \param self The result to clone.
 *
 * \return The cloned result.
 */
static inline pal_helix_result_t*
pal_helix_result_cln(const pal_helix_result_t* self) {
  pal_helix_result_t* clone = malloc(sizeof(pal_helix_result_t));
  pal_helix_result_cpy(clone, self);
  return clone;
}

#endif // __pal_helix_h__

/*! \} */
