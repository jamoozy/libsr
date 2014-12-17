#ifndef __pal_composite_h__
#define __pal_composite_h__

#include <math.h>

#include "paleo.h"

#define PAL_COMP_MAX_REC_DEPTH 4
#define PAL_COMP_MAX_REC_SIZE (exp(2,PAL_COMP_MAX_REC_DEPTH)-1)

typedef struct {
  pal_type_e type;
  void* shape;
} pal_sub_shape_t;

typedef struct {
  pal_sub_shape_t* subs;
  int num_subs;
} pal_composite_t;

typedef struct {
  PAL_TEST_RESULT_UNION;
  pal_composite_t composite;
} pal_composite_result_t;

typedef struct {
  const pal_stroke_t* stroke;
  pal_composite_result_t result;
} pal_composite_context_t;



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Functions -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Initialize the curve test.
void pal_composite_init();

// De-initializes the curve test.
void pal_composite_deinit();

pal_composite_result_t* pal_composite_test(const pal_stroke_t* stroke);


#endif  // __paleo_composite_h__
