
#include <strings.h>
#include <lapacke.h>

#include "curve_test.h"



// The curve test
static curve_test_context_t context;

void curve_test_init() { bzero(&context, sizeof(curve_test_context_t)); }

void curve_test_deinit() { }

void _reset(const paleo_stroke_t* stroke) {
  bzero(&context, sizeof(curve_test_result_t));
  context.stroke = stroke;
  context.result.possible = 1;
}



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Curve Test ------------------------------- //
////////////////////////////////////////////////////////////////////////////////

const curve_test_result_t* curve_test(const paleo_stroke_t* stroke) {
  _reset(stroke);



  return &context.result;
}
