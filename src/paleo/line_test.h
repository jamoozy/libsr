#ifndef __line_test_h__
#define __line_test_h__

#include "paleo.h"

// Context needed to perform the line test.
typedef struct {
  
} line_test_context_t;

// The results specific to a line test.
typedef struct {
  paleo_test_result_t r;   // The common stuff of results.
} line_test_result_t;


// Does the line test on the paleo stroke.
//   stroke: The stroke to test.
line_test_result_t* line_test(paleo_stroke_t* stroke);

// De-initializes the line by freeing its memory.
void line_test_deinit();

#endif  // __line_test_h__
