#ifndef __paleo_composite_h__
#define __paleo_composite_h__

#include <math.h>

#include "paleo.h"

#define PALEO_COMP_MAX_REC_DEPTH 4
#define PALEO_COMP_MAX_REC_SIZE (exp(2,PALEO_COMP_MAX_REC_DEPTH)-1)

typedef struct {
  paleo_type_e type;
  void* shape;
} paleo_sub_shape_t;

typedef struct {
  paleo_sub_shape_t* subs;
  int num_subs;
} paleo_composite_t;

typedef struct {
  PALEO_TEST_RESULT_UNION;
  paleo_composite_t composite;
} paleo_composite_result_t;

typedef struct {
  const paleo_stroke_t* stroke;
  paleo_composite_result_t result;
} paleo_composite_context_t;



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Functions -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Initialize the curve test.
void paleo_composite_init();

// De-initializes the curve test.
void paleo_composite_deinit();

paleo_composite_result_t* paleo_composite_test(const paleo_stroke_t* stroke);


#endif  // __paleo_composite_h__
