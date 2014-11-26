#ifndef  __paleo_h__
#define  __paleo_h__

#include "common/point.h"
#include "common/stroke.h"

#include "thresh.h"



////////////////////////////////////////////////////////////////////////////////
// ------------------------- Special Paleo Geometry ------------------------- //
////////////////////////////////////////////////////////////////////////////////

// A paleo point; just like a normal point, but with paleo-specific info.
typedef struct {
  union {
    point_t p;
    struct {
      long x;
      long y;
      long t;
      long i;
    };
  };
  int corner;    // Is this a corner?
  double dir;    // Direction of stroke here.
  double dy_dx;  // dy/dx wrt last point.
  double sp;     // Speed of pen when drawing this point.
  double curv;   // Curvature at this point.
} paleo_point_t;

// A paleo stroke; just like a normal stroke, but some paleo-specific info.
typedef struct {
  int num_pts;         // Number of points.
  paleo_point_t* pts;  // Points.
  int num_crnrs;       // Number of corners.
  paleo_point_t** crnrs;  // Corners.
  double px_length;    // Length of the stroke in pixels.
  double ndde;         // Normalized Distance between Direction Extremes.
  double dcr;          // Direction Change Ratio.
  double tot_revs;     // Total revolutions.
  short overtraced;    // Whether the stroke is overtraced.
  short closed;        // Whether the shape is closed.
} paleo_stroke_t;

// Common result info for all tests.
typedef struct {
} paleo_test_result_t;




////////////////////////////////////////////////////////////////////////////////
// ------------------------- Paleo Context & Values ------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Types of recognitions returned by paleo_process & paleo_last_type.
typedef enum {
  PALEO_TYPE_UNRUN = -1,
  PALEO_TYPE_DOT,
  PALEO_TYPE_LINE,
  PALEO_TYPE_CIRCLE,
  PALEO_TYPE_ELLIPSE,
  PALEO_TYPE_ARC,
  PALEO_TYPE_CURVE,
  PALEO_TYPE_SPIRAL,
  PALEO_TYPE_HELIX,
  PALEO_TYPE_COMPLEX,
  PALEO_TYPE_INDET
} paleo_type_e;

// The main paleo object.  Keeps track of context.
typedef struct {
  paleo_type_e type;
  paleo_stroke_t* stroke;
} paleo_context_t;



////////////////////////////////////////////////////////////////////////////////
// ------------------- Paleo Global Processing Functions -------------------- //
////////////////////////////////////////////////////////////////////////////////

// Initialize paleo.
void paleo_init();

// De-initialize paleo (free its memory).
void paleo_deinit();

// Processes the stroke, attempting to recognize it as one of the recognizers
// here.
//   stroke: The stroke to recognize.
paleo_type_e paleo_recognize(const stroke_t* stroke);

// Returns the last-returned value from paleo_process(const stroke_t*)
paleo_type_e paleo_last_type();

#endif  //__paleo_h__
