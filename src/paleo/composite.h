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

#define PAL_COMP_MAX_REC_DEPTH 4
#define PAL_COMP_MAX_REC_SIZE (exp(2,PAL_COMP_MAX_REC_DEPTH)-1)

// A sub-shape recognized from a sub-stroke in the composite shape.
typedef struct {
  pal_type_e type;    // The type of this subshape's shape.
  void* shape;        // The subshape.
} pal_sub_shape_t;

// The composite shape.
typedef struct {
  pal_sub_shape_t* subs;  // An array of subshapes.
  int num_subs;           // The number of subshapes.
} pal_composite_t;

// The composite result.
typedef struct {
  PAL_RESULT_UNION;
  pal_composite_t composite;
} pal_composite_result_t;

// The composite context.
typedef struct {
  const pal_stroke_t* stroke;
  pal_composite_result_t result;
} pal_composite_context_t;



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Functions -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Copies the composite shape from 'src' to 'dst'.
//    dst: Destination composite.
//    src: Source composite.
void pal_composite_cpy(pal_composite_t* dst, const pal_composite_t* src);

// Computes the rank of the composite.
//    self: The composite.
int pal_composite_rank(const pal_composite_t* self);

// Determines whether the composite is just a line/pline.
//    self: The composite.
int pal_composite_is_line(const pal_composite_t* self);


// Initialize the curve test.
void pal_composite_init();

// De-initializes the curve test.
void pal_composite_deinit();

// Performs the composite shape test.
//    stroke: The stroke to recognize.
pal_composite_result_t* pal_composite_test(const pal_stroke_t* stroke);

// Does a deep copy of the composite result.
//    dst: The destination composite result.
//    src: The source composite result.
static inline void pal_composite_result_cpy(
    pal_composite_result_t* dst, const pal_composite_result_t* src) {
  memcpy(dst, src, sizeof(pal_composite_result_t));
  pal_composite_cpy(&dst->composite, &src->composite);
}


#endif  // __paleo_composite_h__
