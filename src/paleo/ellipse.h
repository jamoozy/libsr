/*!
 * \addtogroup pal
 * \{
 *
 * \file ellipse.h
 * This file defines the interface for both types of ellipses -- general ones
 * and perfectly round ones (a.k.a. "circles") -- because much of the same
 * functionality is needed for both types of recognizers.
 *
 * \note
 * Much of the ellipse math is based on what I learned from:
 *    http://mathworld.wolfram.com/Ellipse.html
 */

#ifndef __pal_ellipse_h__
#define __pal_ellipse_h__

#include "common/point.h"

#include "paleo.h"


//! A ellipse with some number of joints.
typedef struct {
  point2d_t f1;  //!< The center of the ellipse.
  point2d_t f2;  //!< The center of the ellipse.
  double maj;    //!< Major axis length.
  double min;    //!< Minor axis length.
} pal_ellipse_t;

//! The results specific to an ellipse test.
typedef struct {
  PAL_RESULT_UNION;
  pal_ellipse_t ellipse;  //!< The shape.
} pal_ellipse_result_t;

//! Context needed to perform the ellipse test.
typedef struct {
  const pal_stroke_t* stroke;   //!< The tested stroke.
  double angle;                 //!< Angle traversed about the center with the
                                //!< stroke.
  //! The ideal ellipse.
  struct {
    //! The ideal major axis.
    struct {
      long i;       //!< Index into stroke of one point.
      long j;       //!< Index into stroke of another point.
      double len;   //!< Length of major axis.
    } major;

    //! The ideal minor axis.
    struct {
      point2d_t a;  //!< One minor axis point.
      point2d_t b;  //!< Another minor axis point.
      double len;   //!< Length of minor axis.
    } minor;
    point2d_t center;   //!<  Center of ideal ellipse.
  } ideal;
  pal_ellipse_result_t result;  //!< Result of the ellipse test.
} pal_ellipse_context_t;



//! A circle with some number of joints.
typedef struct {
  point2d_t center;   //!< The center of the circle.
  double r;           //!< The radius of the circle.
} pal_circle_t;

//! The results specific to an ellipse test.
typedef struct {
  PAL_RESULT_UNION;
  pal_circle_t circle;  //!< The shape.
} pal_circle_result_t;

//! Context needed to perform the ellipse test.
typedef struct {
  const pal_stroke_t* stroke;   //!< The stroke to recognize.

  // The ideal ellipse.
  struct {
    point2d_t center;           //!< Center of ideal ellipse.
    double r;                   //!< Radius of ideal circle.
  } ideal;
  pal_circle_result_t result;   //!< Result of the ellipse test.
} pal_circle_context_t;



/*!
 * Creates a new ellipse with the given focci and maj- and min-length
 *
 * \param f1 The first focal point.
 * \param f2 The second focal point.
 * \param maj The length of the major axis.
 * \param min The length of the minor axis.
 *
 * \return The new ellipse.
 */
pal_ellipse_t* pal_ellipse_create(
    const point2d_t* f1, const point2d_t* f2, double maj, double min);

/*!
 * Populate the already-allocated ellipse.
 *
 * \param self The ellipse.
 * \param f1 The first focal point.
 * \param f2 The second focal point.
 * \param maj The length of the major axis.
 * \param min The length of the minor axis.
 */
void pal_ellipse_populate(pal_ellipse_t* self,
    const point2d_t* f1, const point2d_t* f2, double maj, double min);

/*!
 * Does a deep copy of an ellipse.
 *
 * \param dst The destination ellipse.
 * \param src The source ellipse.
 */
#define pal_ellipse_cpy(dst,src) memcpy(dst, src, sizeof(pal_ellipse_t));

/*!
 * Destroys the ellipse by freeing its memory.
 *
 * \param self The ellipse to destroy.
 */
static inline void pal_ellipse_destroy(pal_ellipse_t* self) { free(self); }



/*!
 * Creates a new circle at 0,0 with radius 0.
 *
 * \return The new circle.
 */
pal_circle_t* pal_circle_create();

/*!
 * Creates a circle with the given center and radius.
 *
 * \param r The radius.
 * \param x X coordinate.
 * \param y Y coordinate.
 *
 * \return The new circle.
 */
pal_circle_t* pal_circle_create_full(long r, long x, long y);

/*!
 * Creates a circle with the given center and radius.  The passed center will
 * be taken over by the circle -- in other words, it will not be cloned in
 * this function; it will be freed on a call to circle_destroy(pal_circle_t*).
 *
 * \param r The radius.
 * \param c The center.
 *
 * \return The new circle.
 */
pal_circle_t* pal_circle_create_with_point(long r, const point2d_t* c);

/*!
 * Does a deep copy of a circle.
 *
 * \param dst The destination circle.
 * \param src The source circle.
 */
#define pal_circle_cpy(dst,src) memcpy(dst, src, sizeof(pal_circle_t));

/*!
 * Destroys the circle by freeing its memory.
 *
 * \param self The circle to free.
 */
static inline void pal_circle_destroy(pal_circle_t* self) { free(self); }



/*! Initialize the ellipse test. */
void pal_ellipse_init();

/*! De-initializes the ellipse test by freeing its memory. */
void pal_ellipse_deinit();

/*! Initialize the ellipse test. */
void pal_circle_init();

/*! De-initializes the ellipse test by freeing its memory. */
void pal_circle_deinit();

/*!
 * Does the ellipse test on the paleo stroke.
 *
 * \param stroke The stroke to test.
 */
const pal_ellipse_result_t* pal_ellipse_test(const pal_stroke_t* stroke);

/*!
 * Does a deep copy of an ellipse.
 *
 * \param dst The destination ellipse.
 * \param src The source ellipse.
 */
static inline void pal_ellipse_result_cpy(
    pal_ellipse_result_t* dst, const pal_ellipse_result_t* src) {
  memcpy(dst, src, sizeof(pal_ellipse_t));
}

/*!
 * Clones the ellipse result.  The caller is responsible for freeing the
 * returned memory.
 *
 * \param self The result to clone.
 *
 * \return The cloned result.
 */
static inline pal_ellipse_result_t*
pal_ellipse_result_cln(const pal_ellipse_result_t* self) {
  pal_ellipse_result_t* clone = malloc(sizeof(pal_ellipse_result_t));
  pal_ellipse_result_cpy(clone, self);
  return clone;
}

/*!
 * Destroys the result and frees its memory.
 *
 * \param self The result to free.
 */
static inline void pal_ellipse_result_destroy(pal_ellipse_result_t* self) {
  free(self);
}

/*!
 * Does the circle test on the paleo stroke.
 *
 * \param stroke The stroke to test.
 *
 * \return The result of the test.
 */
const pal_circle_result_t* pal_circle_test(const pal_stroke_t* stroke);

/*!
 * Does a deep copy of a circle.
 *
 * \param dst The destination circle.
 * \param src The source circle.
 */
static inline void pal_circle_result_cpy(
    pal_circle_result_t* dst, const pal_circle_result_t* src) {
  memcpy(dst, src, sizeof(pal_circle_t));
}

/*!
 * Clones the circle result.  The caller is responsible for freeing the returned
 * memory.
 *
 * \param self The result to clone.
 *
 * \return The cloned result.
 */
static inline pal_circle_result_t*
pal_circle_result_cln(const pal_circle_result_t* self) {
  pal_circle_result_t* clone = malloc(sizeof(pal_circle_result_t));
  pal_circle_result_cpy(clone, self);
  return clone;
}

/*!
 * Destroys the result and frees its memory.
 *
 * \param self The result to free.
 */
static inline void pal_circle_result_destroy(pal_circle_result_t* self) {
  free(self);
}

#endif // __pal_ellipse_h__

/*! \} */
