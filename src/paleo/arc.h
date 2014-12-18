#ifndef __pal_arc_h__
#define __pal_arc_h__

#include "common/point.h"
#include "paleo.h"


// A arc with some number of joints.
typedef struct {
  point2d_t pts[2];   // Endpoints of the arc.
  point2d_t center;   // The center of the arc.
  double angle;       // Angle traversed from pts[0] to pts[1].
                      // (implies whether cw or ccw)
} pal_arc_t;

// The results specific to an arc test.
typedef struct {
  PAL_RESULT_UNION;
  double fae;     // Feature Area Error.
  pal_arc_t arc;
} pal_arc_result_t;

// Context needed to perform the arc test.
typedef struct {
  const pal_stroke_t* stroke;
  struct {              // The ideal arc.
    point2d_t center;   // Center of arcllipse.
    double r;           // Arc radius.
    double area;        // Area.
  } ideal;
  pal_arc_result_t result;  // Result of the arc test.
} pal_arc_context_t;


// Creates a new arc with the given
//    p1: The first point.
//    p2: The second point.
//    c: The center of the circle the arc is a part of.
//    angle: The angle to traverse to draw the arc (implies cw vs. ccw).
pal_arc_t* pal_arc_create(const point2d_t* p1, const point2d_t* p2,
    const point2d_t* c, double angle);

// Populates a pre-allocated arc.
//    self: The arc to populate.
//    p1: The first point.
//    p2: The second point.
//    c: The center of the circle the arc is a part of.
//    angle: The angle to traverse to draw the arc (implies cw vs. ccw).
void pal_arc_populate(pal_arc_t* self, const point2d_t* p1, const point2d_t* p2,
    const point2d_t* c, double angle);

// Does a deep copy of an arc.
//    dst: The destination arc.
//    src: The source arc.
#define pal_arc_cpy(dst,src) memcpy(dst, src, sizeof(pal_arc_t));

// Destroys the arc by freeing its memory.
static inline void pal_arc_destroy(pal_arc_t* self) { free(self); }



// Initialize the arc test.
void pal_arc_init();

// De-initializes the arc test by freeing its memory.
void pal_arc_deinit();

// Does the arc test on the paleo stroke.
//   stroke: The stroke to test.
const pal_arc_result_t* pal_arc_test(const pal_stroke_t* stroke);

// Does a deep copy of an arc result.
//    dst: The destination arc result.
//    src: The source arc result.
static inline void
pal_arc_result_cpy(pal_arc_result_t* dst, const pal_arc_result_t* src) {
  memcpy(dst, src, sizeof(pal_arc_t));
}

#endif // __pal_arc_h__
