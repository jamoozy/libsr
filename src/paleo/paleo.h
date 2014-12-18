#ifndef  __pal_h__
#define  __pal_h__

#include "common/point.h"
#include "common/stroke.h"

#include "thresh.h"



////////////////////////////////////////////////////////////////////////////////
// ------------------------- Special Paleo Geometry ------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Common result info for all tests.
//   lse: Least squares error.
//   fa: Feature Area.
//   fmsg: Failure message.
//   possible: Whether the stroke is possible.
#define PAL_RESULT_STRUCT \
struct {            \
  double lse;       \
  double fa;        \
  char*  fmsg;      \
  char   possible;  \
}
typedef PAL_RESULT_STRUCT pal_result_t;
#define PAL_RESULT_UNION union { pal_result_t ptr; PAL_RESULT_STRUCT; };

// Types of recognitions returned by pal_process & pal_last_type.
typedef enum {
  PAL_TYPE_INDET = -2,
  PAL_TYPE_UNRUN = -1,
  PAL_TYPE_DOT,
  PAL_TYPE_LINE,
  PAL_TYPE_PLINE,
  PAL_TYPE_CIRCLE,
  PAL_TYPE_ELLIPSE,
  PAL_TYPE_ARC,
  PAL_TYPE_CURVE,
  PAL_TYPE_SPIRAL,
  PAL_TYPE_HELIX,
  PAL_TYPE_COMPLEX,
  PAL_TYPE_NUM,
} pal_type_e;

#define PAL_TYPE(elem) (_PAL_TYPE_##elem)

typedef enum {
  PAL_MASK_DOT      = 0x001,
  PAL_MASK_LINE     = 0x002,
  PAL_MASK_PLINE    = 0x004,
  PAL_MASK_CIRCLE   = 0x008,
  PAL_MASK_ELLIPSE  = 0x010,
  PAL_MASK_ARC      = 0x020,
  PAL_MASK_CURVE    = 0x040,
  PAL_MASK_SPIRAL   = 0x080,
  PAL_MASK_HELIX    = 0x100,
  PAL_MASK_COMPLEX  = 0x200
} pal_mask_m;

#define PAL_MASK(elem) (_PAL_MASK_##elem)

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

// A single element in the Paleo hierarchy.
typedef struct {
  pal_type_e type;    // Type of this result.
  pal_result_t* res;  // Result here (actual type determined by 'type').
} pal_hier_elem_t;

// The Paleo hierarchy.  This is an array of results ordered by how likely they
// are to be the correct stroke recognition.
typedef struct {
  pal_hier_elem_t r[PAL_TYPE_NUM];   // List of results -- 0 is best result.
  int mask;                           // Bit mask of added hierarchy elements.
  int num;                            // How filled it is.
} pal_hier_t;

// The main Paleo object.  Keeps track of context.
typedef struct {
  pal_stroke_t stroke;    // The Paleo stroke we're recognizing.
  pal_hier_t h;           // The hierarchy we're building.
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

// Gets the last type returned by pal_recognize(const stroke_t*).
pal_type_e pal_last_type();

// Returns the last-returned value from pal_process(const stroke_t*)
const pal_stroke_t* pal_last_stroke();

#endif  //__pal_h__
