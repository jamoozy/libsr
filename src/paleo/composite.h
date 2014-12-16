#ifndef __paleo_composite_h__
#define __paleo_composite_h__

#include "paleo.h"

typedef struct {

} paleo_composite_t;

typedef struct {
  PALEO_TEST_RESULT_UNION;
  paleo_composite_t composite;
} paleo_composite_result_t;

typedef struct {
  const paleo_stroke_t* stroke;
  int recurs;
  struct {
  } ideal;
  paleo_composite_result_t result;
} paleo_composite_context_t;



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Functions -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Initialize the curve test.
void paleo_composite_init();

// De-initializes the curve test.
void paleo_composite_deinit();

const paleo_composite_result_t*
paleo_composite_test(const paleo_stroke_t* stroke);


#endif  // __paleo_composite_h__
