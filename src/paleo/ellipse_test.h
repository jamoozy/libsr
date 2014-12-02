#ifndef __paleo_ellipse_test_h__
#define __paleo_ellipse_test_h__

////////////////////////////////////////////////////////////////////////////////
//   This file handles both types of ellipses -- general ones and perfectly   //
// round ones (a.k.a. "circles") -- because much of the same functionality is //
//                   needed for both types of recognizers.                    //
////////////////////////////////////////////////////////////////////////////////

#include "paleo.h"
#include "ellipse.h"

// The results specific to an ellipse test.
typedef struct {
  PALEO_TEST_RESULT_UNION;
  ellipse_t ellipse;
} ellipse_test_result_t;

// Context needed to perform the ellipse test.
typedef struct {
  char ellipse_run;  // Was ellipse test run?
  const paleo_stroke_t* stroke;
  struct {  // The ideal ellipse.
    struct {
      long i;
      long j;
      double dist;
    } major;
    struct {
      point2d_t a;
      point2d_t b;

      double dist;
    } minor;
    point2d_t center;
  } ideal_ell;
  ellipse_test_result_t *result;  // Result of the ellipse test.
} ellipse_test_context_t;


// Does the ellipse test on the paleo stroke.
//   stroke: The stroke to test.
const ellipse_test_result_t* ellipse_test(const paleo_stroke_t* stroke);

// Does the circle test on the paleo stroke.
//   stroke: The stroke to test.
const ellipse_test_result_t* circle_test(const paleo_stroke_t* stroke);

// Initialize the ellipse test.
void ellipse_test_init();

// De-initializes the ellipse test by freeing its memory.
void ellipse_test_deinit();


#endif  // __paleo_ellipse_test_h__
