/*! \addtogroup pal
 * \{
 *
 * \file arc.h
 * Implementation of interface defined in arc.h.  Together, these files
 * implement the PaleoSketch arc recognizer, shape, and result.
 */


#ifndef __pal_arc_h__
#define __pal_arc_h__

#include "common/point.h"
#include "paleo.h"


//! A arc with some number of joints.
typedef struct {
  point2d_t pts[2];   //!< Endpoints of the arc.
  point2d_t center;   //!< The center of the arc.
  double angle;       //!< Angle traversed from pts[0] to pts[1].
                      //!< (implies whether cw or ccw)
} pal_arc_t;

//! The results specific to an arc test.
typedef struct {
  PAL_RESULT_UNION;
  double fae;     //!< Feature Area Error.
  pal_arc_t arc;  //!< The actual arc.
} pal_arc_result_t;

//! Context needed to perform the arc test.
typedef struct {
  const pal_stroke_t* stroke;  //!< The stroke we're recognizing.

  //!< The ideal arc.
  struct {
    point2d_t center;   //!< Center of arcllipse.
    double r;           //!< Arc radius.
    double area;        //!< Area.
  } ideal;
  pal_arc_result_t result;  //!< Result of the arc test.
} pal_arc_context_t;


/*! Creates a new arc with the given parameters.
 *
 * \param p1 The first point.
 * \param p2 The second point.
 * \param c The center of the circle the arc is a part of.
 * \param angle The angle to traverse to draw the arc (implies cw vs. ccw).
 *
 * \return The arc.
 */
pal_arc_t* pal_arc_create(const point2d_t* p1, const point2d_t* p2,
    const point2d_t* c, double angle);

/*! Populates a pre-allocated arc.
 *
 * \param self The arc to populate.
 * \param p1 The first point.
 * \param p2 The second point.
 * \param c The center of the circle the arc is a part of.
 * \param angle The angle to traverse to draw the arc (implies cw vs. ccw).
 */
void pal_arc_populate(pal_arc_t* self, const point2d_t* p1, const point2d_t* p2,
    const point2d_t* c, double angle);

/*! Does a deep copy of an arc.
 *
 * \param dst The destination arc.
 * \param src The source arc.
 */
#define pal_arc_cpy(dst,src) memcpy(dst, src, sizeof(pal_arc_t));

/*! Destroys the arc by freeing its memory.
 *
 * \param self The stroke to destroy.
 */
static inline void pal_arc_destroy(pal_arc_t* self) { free(self); }



/*! Initialize the arc test. */
void pal_arc_init();

/*! De-initializes the arc test by freeing its memory. */
void pal_arc_deinit();

/*! Does the arc test on the paleo stroke.
 *
 * \param stroke The stroke to test.
 *
 * \return The result of the recognition attempt.
 */
const pal_arc_result_t* pal_arc_test(const pal_stroke_t* stroke);

/*! Does a deep copy of an arc result.
 *
 * \param dst The destination arc result.
 * \param src The source arc result.
 */
static inline void
pal_arc_result_cpy(pal_arc_result_t* dst, const pal_arc_result_t* src) {
  memcpy(dst, src, sizeof(pal_arc_t));
}

#endif // __pal_arc_h__

/*! \} */
