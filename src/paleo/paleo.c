#include <assert.h>
#include <string.h>
#include <math.h>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include "common/util.h"
#include "paleo.h"



//////////////////////////////////////////////////////////////////////////////
// ---------------------------- Paleo Up/Down ----------------------------- //
//////////////////////////////////////////////////////////////////////////////

static paleo_context_t paleo;

void paleo_init() {
  memset(&paleo, 0, sizeof(paleo_context_t));
  paleo.type = PALEO_TYPE_UNRUN;
}

void paleo_deinit() {
  //line_rec_deinit();
  //circle_rec_deinit();
  //ellipse_rec_deinit();
  //arc_rec_deinit();
  //curve_rec_deinit();
  //spiral_rec_deinit();
  //helix_rec_deinit();
  //complex_rec_deinit();
}



//////////////////////////////////////////////////////////////////////////////
// ------------------------- Processing a Stroke -------------------------- //
//////////////////////////////////////////////////////////////////////////////


// Computes speed in px/s between the two points.
static inline double _speed(const point2dt_t* a, const point2dt_t* b) {
  // Should not have a div/0 error because we made sure times didn't match.
  return point2d_distance((point2d_t*)a, (point2d_t*)b) / abs(b->t - a->t);
}

// Determines the simple dy/dx for b given a was the last point in the
// stroke.
//   a: The first point.
//   b: The second point.
static inline double _dy_dx_direction(const point2d_t* a, const point2d_t* b) {
  return (b->y - a->y) / (b->x - a->x);
}

// Based on paper by Bo Yu & Shijie Cai in 2003 entitled:
//   "A Domain-Independent System for Sketch Recognition"
// See page 142 (PDF page 2) for the definition of direction.
//
// This finds the curvature defined by the two given points according to Yu et
// al.'s method.  This function will return the "direction" (defined in the
// paper) of the point at a given its next point b.
static inline double _yu_direction(const point2d_t* a, const point2d_t* b) {
  return atan((b->y - a->y) / (b->x - a->x));
}

#define K 3  // The default K used in the below computation.

// Based on paper by Bo Yu & Shijie Cai in 2003 entitled:
//   "A Domain-Independent System for Sketch Recognition"
// See page 142 (PDF page 2) for the definition of direction.
//
// This finds the curvature given a section of a stroke defined by the window
// size parameter "k" (see the paper for a full definition).  This function
// assumes that there are 2k+1 points in 'sub_strk'.
static inline double _yu_curvature(int k, const paleo_point_t* sub_strk) {
  const int NUM = 2*k+1;

  double diff_sum = 0;  // sum of direction differences.
  double len = 0;       // substroke length
  for (int i = 0; i < NUM; i++) {
    // Add next sub-segment length.
    len += point2d_distance(
        (point2d_t*)&sub_strk[i], (point2d_t*)&sub_strk[i+1]);

    // Find direction difference, normalize, and add to diff_sum.
    double diff = sub_strk[i+1].dir - sub_strk[i].dir;
    while (diff >  M_PI) { diff -= M_PI; }
    while (diff < -M_PI) { diff += M_PI; }
    diff_sum += diff;
  }

  return diff_sum / len;
}

// Computes DCR of paleo.stroke.
static inline void _compute_dcr() {
  assert(paleo.stroke != NULL);

  double prog = 0;  // length-based progress along the stroke
  double first_i = -1, last_i = -1;  // portion of stroke we use
  double avg_d_dir = 0;  // average change in direction
  double max_d_dir = 0;  // maximum change in direction
  for (int i = 1; i < paleo.stroke->num_pts; i++) {
    prog += point2d_distance(
        (point2d_t*)&paleo.stroke->pts[i-1], (point2d_t*)&paleo.stroke->pts[i]);

    if (prog / paleo.stroke->px_length <= 0.05) { continue; }
    if (first_i < 0) { first_i = i; }
    if (prog / paleo.stroke->px_length >= 0.95) {
      last_i = i;
      break;
    }

    double d_dir = abs(paleo.stroke->pts[i-1].dir - paleo.stroke->pts[i].dir);
    if (d_dir > max_d_dir) { max_d_dir = d_dir; }
    avg_d_dir += d_dir;
  }
  avg_d_dir /= last_i - first_i + 1;
  paleo.stroke->dcr = max_d_dir / avg_d_dir;
}

static inline void _break_stroke(int first_i, int last_i) {
  assert(0 <= first_i && first_i < last_i && last_i < paleo.stroke->num_pts);
  paleo.stroke->num_pts = last_i - first_i + 1;
  memmove(paleo.stroke->pts, &paleo.stroke->pts[first_i],
      paleo.stroke->num_pts * sizeof(paleo_point_t));
  paleo.stroke->pts = realloc(paleo.stroke->pts,
      paleo.stroke->num_pts * sizeof(paleo_point_t));
}


// Does pre-processing on a stroke to create a paleo stroke: a stroke with more
// information, used by the individual recognizers.
static void _process_stroke(const stroke_t* strk) {
  paleo_stroke_t* ps = paleo.stroke = calloc(1, sizeof(paleo_stroke_t));
  ps->pts = calloc(strk->num, sizeof(paleo_point_t));

  // PaleoSketch, pg 3, para 1:
  //    "If two consecutive points either have the same x and y values or if
  //     they have the same time value then the second point is removed."
  for (int i = 0; i < strk->num; i++) {
    if (i > 0) {
      paleo_point_t* last = &ps->pts[ps->num_pts-1];
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
    ps->pts = realloc(ps->pts, ps->num_pts * sizeof(paleo_point_t));
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
    ps->pts[i].dir = _yu_direction(
        (point2d_t*)&ps->pts[i], (point2d_t*)&ps->pts[i+1]);

    // I wasn't sure about how to compute speed (I'll have to look more
    // carefully at the Sezgin paper), so I just figured it should be in px/s.
    ps->pts[i].sp = _speed(
        (point2dt_t*)&ps->pts[i], (point2dt_t*)&ps->pts[i+i]);
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

  // Compute length.
  ps->px_length = 0;
  for (int i = 1; i < ps->num_pts; i++) {
    ps->px_length += point2d_distance(
        (point2d_t*)&ps->pts[i-1], (point2d_t*)&ps->pts[i]);
  }

  // Compute dy/dx for each point (to compute NDDE).
  int max_i = 1;
  int min_i = 1;
  for (int i = 1; i < ps->num_pts; i++) {
    ps->pts[i].dy_dx = _dy_dx_direction(
        (point2d_t*)&ps->pts[i-1], (point2d_t*)&ps->pts[i]);
    if (ps->pts[i].dy_dx > ps->pts[max_i].dy_dx) { max_i = i; }
    if (ps->pts[i].dy_dx < ps->pts[min_i].dy_dx) { min_i = i; }
  }

  // Compute length between min and max, then normalize.
  double sub_length = 0;
  if (max_i < min_i) { swap(int, max_i, min_i); }
  for (int i = min_i + 1; i < max_i; i++) {
    sub_length += point2d_distance(
        (point2d_t*)&ps->pts[i-1], (point2d_t*)&ps->pts[i]);
  }
  ps->ndde = sub_length / ps->px_length;

  // Compute DCR.
  _compute_dcr();

  if (ps->num_pts < PALEO_THRESH_B || ps->px_length < PALEO_THRESH_C) {
    // Stroke too small to warrant tail removal.
    return;
  }

  // Trim tails -- find first and last highest curvature.
  int first_i = 0, last_i = ps->num_pts - 1;
  double prog = 0;
  for (int i = 1; i < ps->num_pts - 1; i++) {
    prog += point2d_distance(
        (point2d_t*)&ps->pts[i-1], (point2d_t*)&ps->pts[i]);
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
  ps->tot_revs = (ps->pts[ps->num_pts-1].dir - ps->pts[0].dir) / (2 * M_PI);
  ps->overtraced = ps->tot_revs > PALEO_THRESH_D;

  // Compute closed-ness.
  ps->closed = (point2d_distance(
        (point2d_t*)&ps->pts[0], (point2d_t*)&ps->pts[ps->num_pts-1]) /
      ps->px_length) < PALEO_THRESH_E && ps->tot_revs > PALEO_THRESH_F;
}

paleo_type_e paleo_recognize(const stroke_t* stroke) {
  _process_stroke(stroke);

  // TODO continue

  return paleo.type;
}

paleo_type_e paleo_last_type() { return paleo.type; }

const paleo_stroke_t* paleo_last_stroke() { return paleo.stroke; }
