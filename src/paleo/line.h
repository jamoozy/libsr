#ifndef __pal_line_h__
#define __pal_line_h__

#include "common/point.h"

#include "paleo.h"
#include "line.h"

// A line with some number of joints.
typedef struct {
  size_t num;      // The number of points, including start and end.
  point2d_t* pts;  // The points.  Must be at least 2.
} pal_line_t;

// The results specific to a line test.
typedef struct {
  PALEO_TEST_RESULT_UNION;
  pal_line_t line;
} pal_line_result_t;

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
  pal_line_result_t *result;
} pal_line_context_t;





// Creates a new line from 0,0 to 0,0.
pal_line_t* pal_line_create();

// Creates a line with the given points.
//   a: The starting point of the line.
//   b: The ending point of the line.
pal_line_t* pal_line_create_points_with_points(
    const point2d_t* a, const point2d_t* b);

// Creates a line containing newly created points with the given coordinates.
//   ax: The starting point's X coord.
//   ay: The starting point's Y coord.
//   bx: The ending point's X coord.
//   by: The ending point's Y coord.
pal_line_t* pal_line_create_points_with_longs(
    long ax, long ay, long bx, long by);

// Frees the memory in a line.
//   self: The line to free.
static inline void pal_line_destroy(pal_line_t* self) {
  free(self->pts);
  free(self);
}





// Does the line test on the paleo stroke.
//   stroke: The stroke to test.
const pal_line_result_t* pal_line_test(const pal_stroke_t* stroke);

// Does the poly line test on the paleo stroke.
//   stroke: The stroke to test.
const pal_line_result_t* pal_pline_test(const pal_stroke_t* stroke);

// Initialize the line test.
static inline void line_init() { bzero(&context, sizeof(line_test_context_t)); }

// De-initializes the line test by freeing its memory.
static inline void line_deinit() { free(context.result); }


#endif // __pal_line_h__
