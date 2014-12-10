#ifndef __paleo_arc_test_h__
#define __paleo_arc_test_h__

#include "paleo.h"
#include "arc.h"



////////////////////////////////////////////////////////////////////////////////
// --------------------------- Results & Contexts --------------------------- //
////////////////////////////////////////////////////////////////////////////////

// The results specific to an arc test.
typedef struct {
  PALEO_TEST_RESULT_UNION;
  double fae;     // Feature Area Error.
  arc_t arc;
} arc_test_result_t;

// Context needed to perform the arc test.
typedef struct {
  const paleo_stroke_t* stroke;
  struct {              // The ideal arc.
    point2d_t center;   // Center of arcllipse.
    double r;           // Arc radius.
    double area;        // Area.
  } ideal;
  arc_test_result_t result;  // Result of the arc test.
} arc_test_context_t;



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Functions -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Initialize the arc test.
void arc_test_init();

// De-initializes the arc test by freeing its memory.
void arc_test_deinit();

// Does the arc test on the paleo stroke.
//   stroke: The stroke to test.
const arc_test_result_t* arc_test(const paleo_stroke_t* stroke);


#endif  // __paleo_arc_test_h__
