#ifndef __paleo_curve_test_h__
#define __paleo_curve_test_h__

#include "paleo.h"
#include "curve.h"

typedef struct {
  PALEO_TEST_RESULT_UNION;
  curve_t curve;
} curve_test_result_t;

typedef struct {
  const paleo_stroke_t* stroke;
  double* Xs;
  double* Ys;
  struct {
    point2d_t Cs[4];    // Computed control points
    double lse;         // Least Squares Error
  } ideal_4;
  struct {
    point2d_t Cs[5];    // Computed control points
    double lse;         // Least Squares Error
  } ideal_5;
  curve_test_result_t result;   // Both results, 4 then 5.
} curve_test_context_t;



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Functions -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Initialize the curve test.
void curve_test_init();

// De-initializes the curve test by freeing its memory.
void curve_test_deinit();

// Does the curve test on the paleo stroke.
//   stroke: The stroke to test.
const curve_test_result_t* curve_test(const paleo_stroke_t* stroke);


#endif  // __paleo_curve_test_h__
