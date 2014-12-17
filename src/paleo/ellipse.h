#ifndef __pal_ellipse_h__
#define __pal_ellipse_h__

////////////////////////////////////////////////////////////////////////////////
//   This file handles both types of ellipses -- general ones and perfectly   //
// round ones (a.k.a. "circles") -- because much of the same functionality is //
//                  needed for both types of r;ecognizers.                    //
////////////////////////////////////////////////////////////////////////////////

#include "common/point.h"

#include "paleo.h"

// Much of the ellipse math is based on what I learned from:
//    http://mathworld.wolfram.com/Ellipse.html

// A ellipse with some number of joints.
typedef struct {
  point2d_t f1;  // The center of the ellipse.
  point2d_t f2;  // The center of the ellipse.
  double maj;    // Major axis length.
  double min;    // Minor axis length.
} pal_ellipse_t;

// The results specific to an ellipse test.
typedef struct {
  PAL_TEST_RESULT_UNION;
  pal_ellipse_t ellipse;
} pal_ellipse_result_t;

// Context needed to perform the ellipse test.
typedef struct {
  const pal_stroke_t* stroke;
  double angle;         // Angle traversed about the center with the stroke.
  struct {              // The ideal ellipse.
    struct {
      long i;           // Index into stroke of one point.
      long j;           // Index into stroke of another point.
      double len;       // Length of major axis.
    } major;
    struct {
      point2d_t a;      // One minor axis point.
      point2d_t b;      // Another minor axis point.
      double len;       // Length of minor axis.
    } minor;
    point2d_t center;   // Center of ideal ellipse.
  } ideal;
  pal_ellipse_result_t result;  // Result of the ellipse test.
} pal_ellipse_context_t;



// A circle with some number of joints.
typedef struct {
  point2d_t center;   // The center of the circle.
  double r;           // The radius of the circle.
} pal_circle_t;

// The results specific to an ellipse test.
typedef struct {
  PAL_TEST_RESULT_UNION;
  pal_circle_t circle;
} pal_circle_result_t;

// Context needed to perform the ellipse test.
typedef struct {
  const pal_stroke_t* stroke;
  struct {                      // The ideal ellipse.
    point2d_t center;           // Center of ideal ellipse.
    double r;                   // Radius of ideal circle.
  } ideal;
  pal_circle_result_t result;   // Result of the ellipse test.
} pal_circle_context_t;



// Creates a new ellipse with the given focci and maj- and min-length
pal_ellipse_t* pal_ellipse_create(
    const point2d_t* f1, const point2d_t* f2, double maj, double min);

// Populate the already-allocated ellipse.
void pal_ellipse_populate(pal_ellipse_t* self,
    const point2d_t* f1, const point2d_t* f2, double maj, double min);

// Destroys the ellipse by freeing its memory.
void pal_ellipse_destroy(pal_ellipse_t* self);



// Creates a new circle at 0,0 with radius 0.
pal_circle_t* pal_circle_create();

// Creates a circle with the given center and radius.
//   r: The radius.
//   x: X coordinate.
//   y: Y coordinate.
pal_circle_t* pal_circle_create_full(long r, long x, long y);

// Creates a circle with the given center and radius.  The passed center will
// be taken over by the circle -- in other words, it will not be cloned in
// this function; it will be freed on a call to circle_destroy(pal_circle_t*).
//   r: The radius.
//   c: The center.
pal_circle_t* pal_circle_create_with_point(long r, const point2d_t* c);

// Destroys the circle by freeing its memory.
void pal_circle_destroy(pal_circle_t*);



// Initialize the ellipse test.
void pal_ellipse_init();

// De-initializes the ellipse test by freeing its memory.
void pal_ellipse_deinit();

// Initialize the ellipse test.
void pal_circle_init();

// De-initializes the ellipse test by freeing its memory.
void pal_circle_deinit();

// Does the ellipse test on the paleo stroke.
//   stroke: The stroke to test.
const pal_ellipse_result_t* pal_ellipse_test(const pal_stroke_t* stroke);

// Does the circle test on the paleo stroke.
//   stroke: The stroke to test.
const pal_circle_result_t* pal_circle_test(const pal_stroke_t* stroke);

#endif // __pal_ellipse_h__
