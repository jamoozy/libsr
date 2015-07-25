/*! \file paleo.h
 * The interface file defining the interface to PaleoSketch.  See
 * \cite PaleoSketch.
 *
 * \addtogroup pal
 * \{
 */

#ifndef  __paleo_h__
#define  __paleo_h__

#include "common/point.h"
#include "common/stroke.h"

#include "thresh.h"



////////////////////////////////////////////////////////////////////////////////
// ------------------------- Special Paleo Geometry ------------------------- //
////////////////////////////////////////////////////////////////////////////////

/*! Common result info for all tests.
 * \param lse Least squares error.
 * \param fa Feature Area.
 * \param fmsg Failure message.
 * \param possible Whether the stroke is possible.
 */
#define PAL_RESULT_STRUCT \
struct {            \
  double lse;       \
  double fa;        \
  char*  fmsg;      \
  char   possible;  \
}

//! Paleo result structure.
typedef PAL_RESULT_STRUCT pal_result_t;

/*! The paleo result union is used to extend the structure with custom,
 * shape-specific information.
 *
 * \see PAL_RESULT_STRUCT
 */
#define PAL_RESULT_UNION union { pal_result_t pr; PAL_RESULT_STRUCT; };

//! Types of recognitions returned by pal_process & pal_last_type.
typedef enum {
  PAL_TYPE_INDET = -2,  //!< inteterminate
  PAL_TYPE_UNRUN = -1,  //!< not yet run
  PAL_TYPE_DOT,         //!< dot recognized
  PAL_TYPE_LINE,        //!< pal_line_t (line segment) recognized
  PAL_TYPE_PLINE,       //!< pal_line_t (polyline) recognized
  PAL_TYPE_CIRCLE,      //!< pal_circle_t recognized
  PAL_TYPE_ELLIPSE,     //!< pal_ellipse_t recognized
  PAL_TYPE_ARC,         //!< pal_arc_t recognized
  PAL_TYPE_CURVE,       //!< pal_curve_t recognized
  PAL_TYPE_SPIRAL,      //!< pal_spiral_t recognized
  PAL_TYPE_HELIX,       //!< pal_helix_t recognized
  PAL_TYPE_COMPOSITE,   //!< pal_composite_t recognized
  PAL_TYPE_NUM,         //!< The total number of recognition type.
} pal_type_e;

//! Convenience macro.  Instead of writing: \c PAL_TYPE_LINE you can use: \c
//! PAL_TYPE(LINE)
#define PAL_TYPE(elem) (PAL_TYPE_##elem)

//! The rank of a dot; this is an invalid concept and will \c assert(0).
#define PAL_RANK_DOT        ({assert(0);})
//! The rank of a line.
#define PAL_RANK_LINE       1
//! The rank of a polyline; this must be computed and will \c assert(0).
#define PAL_RANK_PLINE      ({assert(0);})
//! The rank of a circle.
#define PAL_RANK_CIRCLE     5
//! The rank of a ellipse.
#define PAL_RANK_ELLIPSE    5
//! The rank of a arc.
#define PAL_RANK_ARC        3
//! The rank of a curve.
#define PAL_RANK_CURVE      5
//! The rank of a spiral.
#define PAL_RANK_SPIRAL     5
//! The rank of a helix.
#define PAL_RANK_HELIX      5
//! The rank of a composite; this must be computed and will \c assert(0).
#define PAL_RANK_COMPOSITE  ({assert(0);})

/*! Convenience macro for paleo ranks.  Instead of writing: \c PAL_RANK_LINE you
 * can use: \c PAL_RANK(LINE)
 */
#define PAL_RANK(elem) (PAL_RANK_##elem)

//! Bitmask that encodes paleo types.
typedef enum {
  PAL_MASK_DOT        = 0x001,  //!< Dot type.
  PAL_MASK_LINE       = 0x002,  //!< Line type.
  PAL_MASK_PLINE      = 0x004,  //!< Polyline type.
  PAL_MASK_CIRCLE     = 0x008,  //!< Circle type.
  PAL_MASK_ELLIPSE    = 0x010,  //!< Ellipse type.
  PAL_MASK_ARC        = 0x020,  //!< Arc type.
  PAL_MASK_CURVE      = 0x040,  //!< Curve type.
  PAL_MASK_SPIRAL     = 0x080,  //!< Spiral type.
  PAL_MASK_HELIX      = 0x100,  //!< Helix type.
  PAL_MASK_COMPOSITE  = 0x200   //!< Composite type.
} pal_mask_m;

/*! Convenience macro for paleo mask values.  Instead of writing: \c
 * PAL_RANK_LINE you can use: \c PAL_RANK(LINE)
 */
#define PAL_MASK(elem) (PAL_MASK_##elem)

//! A paleo point; just like a normal point, but with paleo-specific info.
typedef struct {
  POINT_UNION;   //!< Inheriting from points.
  int corner;    //!< Is this a corner?
  double dir;    //!< Direction of stroke here.
  double dy_dx;  //!< dy/dx wrt last point.
  double sp;     //!< Speed of pen when drawing this point.
  double curv;   //!< Curvature at this point.
} pal_point_t;

//! A paleo stroke; just like a normal stroke, but some paleo-specific info.
typedef struct {
  int num_pts;            //!< Number of points.
  pal_point_t* pts;       //!< Points.
  int num_crnrs;          //!< Number of corners.
  pal_point_t** crnrs;    //!< Pointers to the points in 'pts' that are corners.
  double px_length;       //!< Length of the stroke in pixels.
  double ndde;            //!< Normalized Distance between Direction Extremes.
  double dcr;             //!< Direction Change Ratio.
  double tot_revs;        //!< Total revolutions.
  short overtraced;       //!< Whether the stroke is overtraced.
  short closed;           //!< Whether the shape is closed.
} pal_stroke_t;

//! A single element in the Paleo hierarchy.
typedef struct {
  pal_type_e type;    //!< Type of this result.
  pal_result_t* res;  //!< Result here (actual type determined by 'type').
} pal_hier_elem_t;

/*! The Paleo hierarchy.  This is an array of results ordered by how likely they
 * are to be the correct stroke recognition.
 */
typedef struct {
  //! List of results -- 0 is best result.
  pal_hier_elem_t elems[PAL_TYPE_NUM];
  int mask;     //!< Bit mask of added hierarchy elements.
  int num;      //!< How filled it is.
} pal_hier_t;

//! The main Paleo object.  Keeps track of context.
typedef struct {
  pal_stroke_t stroke;    //!< The Paleo stroke we're recognizing.
  pal_hier_t h;           //!< The hierarchy we're building.
} pal_context_t;



////////////////////////////////////////////////////////////////////////////////
// ------------------- Paleo Global Processing Functions -------------------- //
////////////////////////////////////////////////////////////////////////////////

//! Initialize paleo.
void pal_init();

//! De-initialize paleo (free its memory).
void pal_deinit();

/*! Processes the stroke, attempting to recognize it as one of the shapes here.
 *
 * \param stroke The stroke to recognize.
 */
pal_type_e pal_recognize(const stroke_t* stroke);

/*! Finds the rank of a specific shape.
 *
 * \param type The type of the shape.
 * \param shape The shape.
 */
int pal_shape_rank(pal_type_e type, const void* shape);

/*! Gets the last type returned by pal_recognize(const stroke_t*). */
pal_type_e pal_last_type();

/*! Returns the last-returned value from pal_process(const stroke_t*) */
const pal_stroke_t* pal_last_stroke();

#endif  //__paleo_h__

/*! \} */
