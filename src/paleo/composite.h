/*! \addtogroup pal
 * \{
 *
 * \file composite.h
 * Defines the interface to PaleoSketch's composite test submodule.
 */

#ifndef __pal_composite_h__
#define __pal_composite_h__

#include <stdio.h>
#include <math.h>

#include "paleo.h"
#include "line.h"
#include "ellipse.h"
#include "arc.h"
#include "curve.h"
#include "spiral.h"
#include "helix.h"

//! Maximum recursive calls this test function will make.
#define PAL_COMP_MAX_REC_DEPTH 4

/*!
 * Amount of memory required to store the results???
 *
 * \todo Verify this is still needed.
 */
#define PAL_COMP_MAX_REC_SIZE (exp(2,PAL_COMP_MAX_REC_DEPTH)-1)

//! A sub-shape recognized from a sub-stroke in the composite shape.
typedef struct {
  pal_type_e type;    //!< The type of this subshape's shape.
  void* shape;        //!< The subshape.
} pal_sub_shape_t;

//! The composite shape.
typedef struct {
  pal_sub_shape_t* subs;  //!< An array of subshapes.
  int num_subs;           //!< The number of subshapes.
} pal_composite_t;

//! The composite result.
typedef struct {
  PAL_RESULT_UNION;
  pal_composite_t composite;  //!< The composite shape.
} pal_composite_result_t;

//! The composite context.
typedef struct {
  const pal_stroke_t* stroke;     //!< The tested stroke.
  pal_composite_result_t result;  //!< The test result.
} pal_composite_context_t;



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Functions -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

/*!
 * Copies the composite shape from \c src to \c dst.
 *
 * \param dst Destination composite.
 * \param src Source composite.
 */
void pal_composite_cpy(pal_composite_t* dst, const pal_composite_t* src);

/*!
 * Computes the rank of the composite.
 *
 * \param self The composite.
 */
int pal_composite_rank(const pal_composite_t* self);

/* Determines whether the composite is just a line/pline.
 *
 * \param self The composite.
 *
 * \returns 1 If the composite is a line, 0 otherwise.
 */
int pal_composite_is_line(const pal_composite_t* self);


/*! Initialize the curve test. */
void pal_composite_init();

/*! De-initializes the curve test. */
void pal_composite_deinit();

/*!
 * Performs the composite shape test.
 *
 * \param stroke The stroke to recognize.
 *
 * \return The test result.
 */
pal_composite_result_t* pal_composite_test(const pal_stroke_t* stroke);

/*!
 * Does a deep copy of the composite result.
 *
 * \param dst The destination composite result.
 * \param src The source composite result.
 */
static inline void pal_composite_result_cpy(
    pal_composite_result_t* dst, const pal_composite_result_t* src) {
  memcpy(dst, src, sizeof(pal_composite_result_t));
  pal_composite_cpy(&dst->composite, &src->composite);
}

/*!
 * Completely clones a composite result.  The caller is responsible for freeing
 * the newly allocated, returned result.
 *
 * \param self The composite result to clone.
 *
 * \return The cloned result.
 */
static inline pal_composite_result_t*
pal_composite_result_cln(const pal_composite_result_t* self) {
  pal_composite_result_t* clone = malloc(sizeof(pal_composite_result_t));
  pal_composite_result_cpy(clone, self);
  return clone;
}

#endif  // __paleo_composite_h__

/*! \} */
