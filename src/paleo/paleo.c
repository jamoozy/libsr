#include <config.h>

#include <assert.h>
#include <string.h>
#include <strings.h>
#include <math.h>

#include "common/util.h"

#include "paleo.h"

#include "line.h"     // Also poly-line.
#include "ellipse.h"  // Also circle.
#include "arc.h"
#include "curve.h"
#include "spiral.h"
#include "helix.h"
#include "composite.h"



// Finds the type that Paleo thinks the stroke is.
#define TYPE() (paleo.h.r[0].type)

#define TYPE_ADDED(type) (paleo.h.mask & PAL_MASK(type))

// Adds the result to the hierarchy at the specified location without doing any
// checks.
//    i: The index to add the result at.
//    type: The type of the result.
//    res: The result.
#define Add_H_AT(i, type, res) do {             \
  paleo.h.r[i].res = calloc(1, sizeof(res));    \
  memcpy(&paleo.h.r[i].res, &res, sizeof(res)); \
  paleo.h.mask &= PAL_MASK(type);               \
  paleo.h.num++;                                \
} while (0)

// Checks that the type of the result hasn't already been added before adding it
// to the top of the hierarchy.
//    type: The type of the result.
//    res: The result.
#define PUSH_H(type, res) do {                        \
  if (!TYPE_ADDED(type)) {                            \
    memmove(&paleo.h.r[1], &paleo.h.r[0],             \
        (PAL_TYPE(NUM)-1) * sizeof(pal_hier_elem_t)); \
    ADD_H_AT(0, type, res);                           \
  }                                                   \
} while (0)

// Checks that the type of the result hasn't already been added before adding it
// to the end of the hierarchy.
//    type: The type of the result.
//    res: The result.
#define ENQ_H(type, res) do {         \
  if (!TYPE_ADDED(type)) {            \
    ADD_H_AT(paleo.h.num, type, res); \
  }                                   \
} while(0)


//////////////////////////////////////////////////////////////////////////////
// ---------------------------- Paleo Up/Down ----------------------------- //
//////////////////////////////////////////////////////////////////////////////

static pal_context_t paleo;

void _hier_init(pal_hier_t* h) {
  bzero(h, sizeof(pal_hier_t));
  for (int i = 0; i < PAL_TYPE_NUM; i++) {
    paleo.h.r[i].type = PAL_TYPE_UNRUN;
  }
}

void pal_init() {
  bzero(&paleo, sizeof(pal_context_t));

  pal_line_init();
  pal_ellipse_init();
  pal_circle_init();
  pal_arc_init();
  pal_curve_init();
  pal_spiral_init();
  pal_helix_init();
  pal_composite_init();
}

void pal_deinit() {
  pal_line_deinit();
  pal_ellipse_deinit();
  pal_circle_deinit();
  pal_arc_deinit();
  pal_curve_deinit();
  pal_spiral_deinit();
  pal_helix_deinit();
  pal_composite_deinit();

  free(paleo.stroke.pts);
  free(paleo.stroke.crnrs);
}



//////////////////////////////////////////////////////////////////////////////
// ------------------------- Processing a Stroke -------------------------- //
//////////////////////////////////////////////////////////////////////////////


// Computes speed in px/s between the two points.
static inline double _speed(const point2dt_t* a, const point2dt_t* b) {
  // Should not have a div/0 error because we made sure times didn't match.
  return point2d_distance((point2d_t*)a, (point2d_t*)b) / abs(b->t - a->t);
}

// Based on paper by Bo Yu & Shijie Cai in 2003 entitled:
//   "A Domain-Independent System for Sketch Recognition"
// See page 142 (PDF page 2) for the definition of direction.
//
// This finds the curvature defined by the two given points according to Yu et
// al.'s method.  This function will return the "direction" (defined in the
// paper) of the point at a given its next point b.
static inline double _yu_direction(const point2d_t* a, const point2d_t* b);

// Based on paper by Bo Yu & Shijie Cai in 2003 entitled:
//   "A Domain-Independent System for Sketch Recognition"
// See page 142 (PDF page 2) for the definition of direction.
//
// This finds the curvature given a section of a stroke defined by the window
// size parameter "k" (see the paper for a full definition).  This function
// assumes that there are 2k+1 points in 'sub_strk'.
static inline double _yu_curvature(int k, const pal_point_t* sub_strk);

// Determines the simple dy/dx for b given a was the last point in the
// stroke.
//   a: The first point.
//   b: The second point.
static inline double _dy_dx_direction(const point2d_t* a, const point2d_t* b);

// Based on PaleoSketch paper -- 2nd to last and final paragraphs.
//
// Finds corners in the paleo stroke by iteratively merging close corners and finding the most curved region to reassign the corner to.
static inline void _paulson_corners();

#define K 3  // The default K used in the below computation.

// Computes DCR of paleo.stroke.
static inline void _compute_dcr();

// Breaks the stroke's tails off.
//   first_i: The index of the first point (incl.).
//   last_i: The index of the last point (incl.).
static inline void _break_stroke(int first_i, int last_i);

// Does pre-processing on a stroke to create a paleo stroke: a stroke with more
// information, used by the individual recognizers.
static void _process_stroke(const stroke_t* strk) {
  pal_stroke_t* ps = &paleo.stroke;
  ps->pts = calloc(strk->num, sizeof(pal_point_t));

  // PaleoSketch, pg 3, para 1:
  //    "If two consecutive points either have the same x and y values or if
  //     they have the same time value then the second point is removed."
  for (int i = 0; i < strk->num; i++) {
    if (i > 0) {
      pal_point_t* last = &ps->pts[ps->num_pts-1];
      if (last->p.t == strk->pts[i].t ||
          (last->p.x == strk->pts[i].x && last->p.y == strk->pts[i].y)) {
        continue;  // Same time or same coords, so skipping.
      }
    }

    // Got this far, so okay to add this point (also, update ps->num_pts).
    memcpy(&ps->pts[ps->num_pts++], &strk->pts[i], sizeof(point_t));
  }

  // Cut down on memory use (there may have been removed points).
  if (ps->num_pts < strk->num) {
    ps->pts = realloc(ps->pts, ps->num_pts * sizeof(pal_point_t));
  }

  // PaleoSketch, pg 3, para 2:
  //    "Next, a series of graphs and values are computed for the stroke,
  //     including direction graph, speed graph, curvature graph, and corners.
  //     The graphs are computed using methods from [10] and [12], whereas
  //     corners are calculated using a simple corner finding algorithm presented
  //     in the appendix."
  //
  // Citations:
  //   [10] Sezgin, T.M., Stahovich, T. and Davis, R. Sketch Based Interfaces:
  //        Early Processing for Sketch Understanding. In Proc. of the 2001
  //        Workshop on Perceptive User Interfaces, ACM Press (2001), 1-8.
  //   [12] Yu, B. and Cai, S. A Domain-Independent System for Sketch
  //        Recognition. In Proc. of the 1 st International Conference on
  //        Computer Graphics and Interactive Techniques in Australasia and
  //        South East Asia, ACM Press (2003),141-146.

  // First compute the direction as in Yu et al.'s paper:
  for (int i = 0; i < ps->num_pts - 1; i++) {
    ps->pts[i].dir = _yu_direction(&ps->pts[i].p2d, &ps->pts[i+1].p2d);

    // Correct, if there's never a jump where, |d jump| > pi.  This
    // normalization process ensures that the direction graph is as smooth as it
    // can be given the changes in stroke direction that are common with
    // freehand drawing.  Shape tests assume that the graph will be smooth in
    // this way.
    if (i > 0) {   // else, array O.O.B. error.
      while (ps->pts[i].dir - ps->pts[i-1].dir > M_PIl) {
        ps->pts[i].dir += 2 * M_PIl;
      }
      while (ps->pts[i].dir - ps->pts[i-1].dir < -M_PIl) {
        ps->pts[i].dir -= 2 * M_PIl;
      }
    }

    // I wasn't sure about how to compute speed (I'll have to look more
    // carefully at the Sezgin paper), so I just figured it should be in px/s.
    ps->pts[i].sp = _speed(&ps->pts[i].p2dt, &ps->pts[i+i].p2dt);
  }

  // Next compute the curvature (based on direction).
  for (int i = 1; i < ps->num_pts - 1; i++) {
    ps->pts[i].curv = _yu_curvature(
        ((K < i) ?
            ((K < ps->num_pts - i - 1) ?
                K :
                ps->num_pts - i - 1) :
            i),
        &ps->pts[i]);
  }

  _paulson_corners();

  // Compute length.
  ps->px_length = 0;
  for (int i = 1; i < ps->num_pts; i++) {
    ps->px_length += point2d_distance(&ps->pts[i-1].p2d, &ps->pts[i].p2d);
  }

  // Compute dy/dx for each point (to compute NDDE).
  int max_i = 1;
  int min_i = 1;
  for (int i = 1; i < ps->num_pts; i++) {
    ps->pts[i].dy_dx = _dy_dx_direction(&ps->pts[i-1].p2d, &ps->pts[i].p2d);
    if (ps->pts[i].dy_dx > ps->pts[max_i].dy_dx) { max_i = i; }
    if (ps->pts[i].dy_dx < ps->pts[min_i].dy_dx) { min_i = i; }
  }

  // Compute length between min and max, then normalize.
  double sub_length = 0;
  if (max_i < min_i) { swap(int, max_i, min_i); }
  for (int i = min_i + 1; i < max_i; i++) {
    sub_length += point2d_distance(&ps->pts[i-1].p2d, &ps->pts[i].p2d);
  }
  ps->ndde = sub_length / ps->px_length;

  // Compute DCR.
  _compute_dcr();

  if (ps->num_pts < PAL_THRESH_B || ps->px_length < PAL_THRESH_C) {
    // Stroke too small to warrant tail removal.
    return;
  }

  // Trim tails -- find first and last highest curvature.
  int first_i = 0, last_i = ps->num_pts - 1;
  double prog = 0;
  for (int i = 1; i < ps->num_pts - 1; i++) {
    prog += point2d_distance(&ps->pts[i-1].p2d, &ps->pts[i].p2d);
    double prog_pct = prog / ps->px_length;

    if (prog_pct < 0.20) {  // Scanning for first tail ...
      if (ps->pts[first_i].curv < ps->pts[i].curv) {
        first_i = i;
      }
    } else if (0.20 < prog_pct && prog_pct < 0.80) {
      continue;
    } else {  // Scanning for last tail ...
      if (ps->pts[last_i].curv < ps->pts[i].curv) {
        last_i = i;
      }
    }
  }
  _break_stroke(first_i, last_i);

  // Compute total rotation & whether it's overtraced.
  ps->tot_revs = (ps->pts[ps->num_pts-1].dir - ps->pts[0].dir) / (2 * M_PIl);
  ps->overtraced = ps->tot_revs > PAL_THRESH_D;

  // Compute closed-ness.
  ps->closed = (point2d_distance(&ps->pts[0].p2d, &ps->pts[ps->num_pts-1].p2d) /
      ps->px_length) < PAL_THRESH_E && ps->tot_revs > PAL_THRESH_F;
}

static inline double _yu_direction(const point2d_t* a, const point2d_t* b) {
  return atan((b->y - a->y) / (b->x - a->x));
}

static inline double _yu_curvature(int k, const pal_point_t* sub_strk) {
  const int NUM = 2*k+1;

  double diff_sum = 0;  // sum of direction differences.
  double len = 0;       // substroke length
  for (int i = 0; i < NUM; i++) {
    // Add next sub-segment length.
    len += point2d_distance(
        (point2d_t*)&sub_strk[i], (point2d_t*)&sub_strk[i+1]);

    // Find direction difference, normalize, and add to diff_sum.
    double diff = sub_strk[i+1].dir - sub_strk[i].dir;
    while (diff >  M_PIl) { diff -= M_PIl; }
    while (diff < -M_PIl) { diff += M_PIl; }
    diff_sum += diff;
  }

  return diff_sum / len;
}

static inline double _dy_dx_direction(const point2d_t* a, const point2d_t* b) {
  return (b->y - a->y) / (b->x - a->x);
}

// Merges corners sufficiently close together.  Returns whether something was
// changed in the crnrs array.
static inline short _paulson_merge_corners();

// Find highest curvature in each corner's neighbor hood and set that as the
// corner.  Returns whether something was changed in the crnrs array.
static inline short _paulson_replace_corners();

static inline void _paulson_corners() {
  assert(paleo.stroke.num_crnrs == 0);
  assert(paleo.stroke.crnrs == NULL);

#define _pal_add_to_corners(i) \
  (paleo.stroke.crnrs[paleo.stroke.num_crnrs++] = &paleo.stroke.pts[(i)])

  // init corners with 0th point.
  paleo.stroke.num_crnrs = 0;
  paleo.stroke.crnrs = realloc(paleo.stroke.crnrs,
      paleo.stroke.num_pts * sizeof(pal_point_t*));
  _pal_add_to_corners(0);

  pal_point_t* last = &paleo.stroke.pts[0];
  double px_length = 0;
  for (int i = 1; i < paleo.stroke.num_pts - 1; i++) {
    px_length += point2d_distance(
        &paleo.stroke.pts[i-1].p2d, &paleo.stroke.pts[i].p2d);

    // Are we un-line-like enough?
    if (point2d_distance(
          &last->p2d, &paleo.stroke.pts[i].p2d) > PAL_THRESH_Y) {
      _pal_add_to_corners(i-1);
      last = &paleo.stroke.pts[i];
      px_length = 0;
    }
  }

  _pal_add_to_corners(paleo.stroke.num_pts-1);
  paleo.stroke.crnrs = realloc(paleo.stroke.crnrs,
      paleo.stroke.num_crnrs * sizeof(pal_point_t*));

#undef _pal_add_to_corners

  // Merge corners and replace with highest in region until no change.
  while(_paulson_merge_corners() || _paulson_replace_corners());
}

static inline short _paulson_merge_corners() {
  short rtn = 0;
  for (int c = 1; c < paleo.stroke.num_crnrs; c++) {
    if (paleo.stroke.crnrs[c-1]->p.i + PAL_THRESH_Z * paleo.stroke.num_pts <=
        paleo.stroke.crnrs[c]->p.i) {  // Sufficiently close to be merged.
      rtn = 1;
      if (c == 1) {   // 0th point: just remove other point.
        memmove(&paleo.stroke.crnrs[1], &paleo.stroke.crnrs[2],
            (paleo.stroke.num_crnrs-2) * sizeof(pal_point_t*));
        paleo.stroke.crnrs = realloc(paleo.stroke.crnrs,
            --paleo.stroke.num_crnrs * sizeof(pal_point_t*));
        c--;
      } else if (c == paleo.stroke.num_crnrs - 1) {  // Last point:
        // Just remove other point.
        memmove(&paleo.stroke.crnrs[paleo.stroke.num_crnrs-2],
            &paleo.stroke.crnrs[paleo.stroke.num_crnrs-1],
            sizeof(pal_point_t*));
        paleo.stroke.crnrs = realloc(paleo.stroke.crnrs,
            --paleo.stroke.num_crnrs * sizeof(pal_point_t*));
        c--;
      } else if (c >= paleo.stroke.num_crnrs) {
        assert(0);
      } else {
        const int avg_i =
          (paleo.stroke.crnrs[c-1]->p.i + paleo.stroke.crnrs[c]->p.i) / 2;
        paleo.stroke.crnrs[c-1] = &paleo.stroke.pts[avg_i];
        memmove(&paleo.stroke.crnrs[c], &paleo.stroke.crnrs[c+1],
            (paleo.stroke.num_crnrs - c - 1) * sizeof(pal_point_t*));
        paleo.stroke.crnrs = realloc(paleo.stroke.crnrs,
            --paleo.stroke.num_crnrs * sizeof(pal_point_t*));
        c--;
      }
    }
  }
  return rtn;
}

static inline short _paulson_replace_corners() {
  const int range = (int)ceil(paleo.stroke.num_pts * PAL_THRESH_Z);
  short rtn = 0;
  for (int c = 0; c < paleo.stroke.num_crnrs; c++) {
    pal_point_t* corner = paleo.stroke.crnrs[c];
    for (int i = corner->p.i - range;
        i < MIN(corner->p.i + range, paleo.stroke.num_pts); i++) {
      if (paleo.stroke.pts[i].curv > paleo.stroke.crnrs[c]->curv) {
        paleo.stroke.crnrs[c] = &paleo.stroke.pts[i];
        rtn = 1;
      }
    }
  }
  return rtn;
}

static inline void _compute_dcr() {
  double prog = 0;  // length-based progress along the stroke
  double first_i = -1, last_i = -1;  // portion of stroke we use
  double avg_d_dir = 0;  // average change in direction
  double max_d_dir = 0;  // maximum change in direction
  for (int i = 1; i < paleo.stroke.num_pts; i++) {
    prog += point2d_distance(
        &paleo.stroke.pts[i-1].p2d, &paleo.stroke.pts[i].p2d);

    if (prog / paleo.stroke.px_length <= 0.05) { continue; }
    if (first_i < 0) { first_i = i; }
    if (prog / paleo.stroke.px_length >= 0.95) {
      last_i = i;
      break;
    }

    double d_dir = abs(paleo.stroke.pts[i-1].dir - paleo.stroke.pts[i].dir);
    if (d_dir > max_d_dir) { max_d_dir = d_dir; }
    avg_d_dir += d_dir;
  }
  avg_d_dir /= last_i - first_i + 1;
  paleo.stroke.dcr = max_d_dir / avg_d_dir;
}

static inline void _break_stroke(int first_i, int last_i) {
  // Sanity check.
  assert(0 <= first_i && first_i < last_i && last_i < paleo.stroke.num_pts);

  // Trim off tails.
  paleo.stroke.num_pts = last_i - first_i + 1;
  memmove(paleo.stroke.pts, &paleo.stroke.pts[first_i],
      paleo.stroke.num_pts * sizeof(pal_point_t));
  paleo.stroke.pts = realloc(paleo.stroke.pts,
      paleo.stroke.num_pts * sizeof(pal_point_t));

  // Correct point index's.
  for (int i = 0; i < paleo.stroke.num_pts; i++) {
    paleo.stroke.pts[i].p.i = i;
  }
}


pal_type_e pal_recognize(const stroke_t* stroke) {
  // Process simple stroke to create Paleo stroke.
  _process_stroke(stroke);

  // Create a structure to hold all the 
  struct {
    pal_line_result_t line;
    pal_line_result_t pline;
    pal_ellipse_result_t ellipse;
    pal_circle_result_t circle;
    pal_arc_result_t arc;
    pal_curve_result_t curve;
    pal_spiral_result_t spiral;
    pal_helix_result_t helix;
    pal_composite_result_t composite;
  } r;

  // Run each test in turn, copying over the result into the Paleo object.
  pal_line_result_cpy(&r.line, pal_line_test(&paleo.stroke));
  pal_line_result_cpy(&r.pline, pal_pline_test(&paleo.stroke));
  pal_ellipse_result_cpy(&r.ellipse, pal_ellipse_test(&paleo.stroke));
  pal_circle_result_cpy(&r.circle, pal_circle_test(&paleo.stroke));
  pal_arc_result_cpy(&r.arc, pal_arc_test(&paleo.stroke));
  pal_curve_result_cpy(&r.curve, pal_curve_test(&paleo.stroke));
  pal_spiral_result_cpy(&r.spiral, pal_spiral_test(&paleo.stroke));
  pal_helix_result_cpy(&r.helix, pal_helix_test(&paleo.stroke));
  pal_composite_result_cpy(&r.composite, pal_composite_test(&paleo.stroke));

  // Go through a hierarchy to determine which shape should be the final one.
  //
  // XXX -- A shortcut is taken w.r.t. how spirals and helices are handled; from
  // the paper:
  //
  //    Helixes and spirals are hard to assign scores because they are
  //    arbitrarily large and the number of rotations differs across each
  //    occurrence. Therefore, we gave them a default score of 5.
  //
  // Future versions of this library should improve upon this method.

  // Do hierarchy.
  _hier_init(&paleo.h);

  ENQ_H(LINE, r.line);
  ENQ_H(PLINE, r.pline);
//  ENQ_H(

  return TYPE();
}

pal_type_e pal_last_type() { return TYPE(); }

const pal_stroke_t* pal_last_stroke() { return &paleo.stroke; }
