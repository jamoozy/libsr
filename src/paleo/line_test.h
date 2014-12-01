#ifndef __line_test_h__
#define __line_test_h__

////////////////////////////////////////////////////////////////////////////////
//   This file handles both types of lines -- simple single line segments and //
// lines with multiple joints (called poly lines) -- because much of the same //
//            functionality is needed for both types of recognizers.          //
////////////////////////////////////////////////////////////////////////////////

#include "paleo.h"
#include "line.h"

// The results specific to a line test.
typedef struct {
  PALEO_TEST_RESULT_UNION;
  line_t line;
} line_test_result_t;

// Context needed to perform the line test.
typedef struct {
  const paleo_stroke_t* stroke;
  struct {  // The ideal line.
    double slope;  // slope
    double y_int;  // Y-intercept
    point2d_t p0;  // First point in the ideal line.
    double theta;  // Angle of the line (from p0).
  } ideal_line;

  // Result of all line tests performed by the last call to line_set_test or
  // poly_line_test.  For the simple line test there will be only one result
  // here -- the result of the single line test performed.  For the poly line,
  // there will be num_crnrs tests here.  The 0th will be the final poly line
  // test, and the nth will be the line segment test for the nth subsegment of
  // the line.
  line_test_result_t *result;
} line_test_context_t;


// Does the line test on the paleo stroke.
//   stroke: The stroke to test.
const line_test_result_t* line_seg_test(const paleo_stroke_t* stroke);

// Does the poly line test on the paleo stroke.
//   stroke: The stroke to test.
const line_test_result_t* poly_line_test(const paleo_stroke_t* stroke);

// Initialize the line test.
void line_test_init();

// De-initializes the line test by freeing its memory.
void line_test_deinit();

#endif  // __line_test_h__
