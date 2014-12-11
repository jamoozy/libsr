#ifndef __paleo_spiral_test_h__
#define __paleo_spiral_test_h__

#include "common/point.h"
#include "paleo.h"
#include "spiral.h"

typedef struct {
  PALEO_TEST_RESULT_UNION;
  spiral_t spiral;
} paleo_spiral_test_result_t;

typedef struct {
  const paleo_stroke_t* stroke;
  struct {
    point2d_t center;
    double r;
  } ideal;
  paleo_spiral_test_result_t result;
} paleo_spiral_test_context_t;



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Functions -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Initialize the curve test.
void paleo_spiral_test_init();

// De-initializes the curve test.
void paleo_spiral_test_deinit();

const paleo_spiral_test_result_t*
paleo_spiral_test(const paleo_stroke_t* stroke);

#endif  // __paleo_spiral_test_h__
