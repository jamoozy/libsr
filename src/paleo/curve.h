#ifndef __pal_curve_h__
#define __pal_curve_h__

#include "common/point.h"

#include "paleo.h"
#include "curve.h"


#define CURVE_CONTROL_POINT_CAP 5

// A Bezier curve with some number of control points.
typedef struct {
  point2d_t pts[CURVE_CONTROL_POINT_CAP];   // The control points.
  int num;                                  // The number of control points
} pal_curve_t;

// The recognition result for the curve test.
typedef struct {
  PAL_RESULT_UNION;
  pal_curve_t curve;    // The built curve.
} pal_curve_result_t;

typedef struct {
  const pal_stroke_t* stroke;
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
  pal_curve_result_t result;   // Both results, 4 then 5.
} pal_curve_context_t;



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Functions -------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Creates a new curve at with the specified number of points, all of which
// are at 0,0.
//   num: The number of points in the curve.
pal_curve_t* pal_curve_create(long num);

// Creates a curve with newly created points at the specified coordinates.
//   num: The number of points in the curve.
//   xs: X coordinates.
//   ys: Y coordinates.
pal_curve_t* pal_curve_create_coords(long num, long* xs, long* ys);

// Creates a curve with the given points.
//   num: The number of points.
//   points: The points.
pal_curve_t* pal_curve_create_points(long num, const point2d_t* points);

// Computes 'num' evenly separated points the Bézier curve.
void pal_curve_compute_points(pal_curve_t* self, point2d_t* out, int num);

// Does a deep copy of an curve.
//    dst: The destination curve .
//    src: The source curve.
#define pal_curve_cpy(dst,src) memcpy(dst, src, sizeof(pal_curve_t));

// Destroys the curve by freeing its memory.
static inline void pal_curve_destroy(pal_curve_t* self) { free(self); }

// Recursively computes a point on a Bézier curve.
//    p: The return point.
//    pts: The control points.
//    num_pts: The number of control points.
//    t: The parameter to the Bézier function.
void pal_curve_util_compute_point(point2d_t *p,
    const point2d_t* pts, int num_pts, double t);



// Initialize the curve test.
void pal_curve_init();

// De-initializes the curve test by freeing its memory.
void pal_curve_deinit();

// Does the curve test on the Paleo stroke.
//   stroke: The stroke to test.
const pal_curve_result_t* pal_curve_test(const pal_stroke_t* stroke);

// Does a deep copy of a curve.
//    dst: The destination curve.
//    src: The source curve.
static inline void
pal_curve_result_cpy(pal_curve_result_t* dst, const pal_curve_result_t* src) {
  memcpy(dst, src, sizeof(pal_curve_t));
}


#endif // __pal_curve_h__
