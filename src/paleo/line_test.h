#ifndef __line_test_h__
#define __line_test_h__

#include "paleo.h"

// The results specific to a line test.
typedef struct {
  PALEO_TEST_RESULT_UNION
} line_test_result_t;

// Context needed to perform the line test.
typedef struct {
  const paleo_stroke_t* stroke;
  struct {  // The ideal line.
    double slope;
    double y_int;
    point2d_t p0;
    double theta;
  } ideal_line;
  line_test_result_t* result;
} line_test_context_t;


// Does the line test on the paleo stroke.
//   stroke: The stroke to test.
line_test_result_t* line_test(const paleo_stroke_t* stroke);

// De-initializes the line by freeing its memory.
void line_test_deinit();

#endif  // __line_test_h__
