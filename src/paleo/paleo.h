#ifndef  __pal_h__
#define  __pal_h__

#include "common/point.h"
#include "common/stroke.h"

#include "thresh.h"



////////////////////////////////////////////////////////////////////////////////
// ------------------------- Special Paleo Geometry ------------------------- //
////////////////////////////////////////////////////////////////////////////////

// A paleo point; just like a normal point, but with paleo-specific info.
typedef struct {
  POINT_UNION;   // Inheriting from points.
  int corner;    // Is this a corner?
  double dir;    // Direction of stroke here.
  double dy_dx;  // dy/dx wrt last point.
  double sp;     // Speed of pen when drawing this point.
  double curv;   // Curvature at this point.
} pal_point_t;

// A paleo stroke; just like a normal stroke, but some paleo-specific info.
typedef struct {
  int num_pts;            // Number of points.
  pal_point_t* pts;       // Points.
  int num_crnrs;          // Number of corners.
  pal_point_t** crnrs;    // Corners.
  double px_length;       // Length of the stroke in pixels.
  double ndde;            // Normalized Distance between Direction Extremes.
  double dcr;             // Direction Change Ratio.
  double tot_revs;        // Total revolutions.
  short overtraced;       // Whether the stroke is overtraced.
  short closed;           // Whether the shape is closed.
} pal_stroke_t;

// Common result info for all tests.
//   possible: Whether the stroke is possible.
//   lse: Least squares error.
//   fa: Feature Area.
#define PAL_TEST_RESULT_STRUCT \
struct {            \
  double lse;       \
  double fa;        \
  char*  fmsg;      \
  char   possible;  \
}
typedef PAL_TEST_RESULT_STRUCT pal_test_result_t;
#define PAL_TEST_RESULT_UNION \
  union { pal_test_result_t ptr; PAL_TEST_RESULT_STRUCT; };



////////////////////////////////////////////////////////////////////////////////
// ------------------------- Paleo Context & Values ------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Types of recognitions returned by pal_process & pal_last_type.
typedef enum {
  PAL_TYPE_UNRUN = -1,
  PAL_TYPE_DOT,
  PAL_TYPE_LINE,
  PAL_TYPE_CIRCLE,
  PAL_TYPE_ELLIPSE,
  PAL_TYPE_ARC,
  PAL_TYPE_CURVE,
  PAL_TYPE_SPIRAL,
  PAL_TYPE_HELIX,
  PAL_TYPE_COMPLEX,
  PAL_TYPE_INDET
} pal_type_e;

// The main paleo object.  Keeps track of context.
typedef struct {
  pal_type_e type;
  pal_stroke_t* stroke;
} pal_context_t;



////////////////////////////////////////////////////////////////////////////////
// ------------------- Paleo Global Processing Functions -------------------- //
////////////////////////////////////////////////////////////////////////////////

// Initialize paleo.
void pal_init();

// De-initialize paleo (free its memory).
void pal_deinit();

// Processes the stroke, attempting to recognize it as one of the recognizers
// here.
//   stroke: The stroke to recognize.
pal_type_e pal_recognize(const stroke_t* stroke);

// Returns the last-returned value from pal_process(const stroke_t*)
pal_type_e pal_last_type();

#endif  //__pal_h__
