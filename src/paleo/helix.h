#ifndef __pal_helix_h__
#define __pal_helix_h__

#include "common/point.h"

#include "paleo.h"

// The helix shape.
typedef struct {
  point2d_t c[2];   // Starting and ending center points.
  double r;         // Radius.
  double theta_i;   // Initial angle.
  double theta_t;   // Angle traversed.
  int cw;           // Clockwise?
} pal_helix_t;

// The helix test result.
typedef struct {
  PAL_RESULT_UNION;
  pal_helix_t helix;
} pal_helix_result_t;

// The helix test context.
typedef struct {
  const pal_stroke_t* stroke;
  pal_helix_result_t result;
} pal_helix_context_t;



// Creates a new helix.
pal_helix_t* helix_create();

// Destroys the helix by freeing its memory.
//    self: The helix to destroy.
void pal_helix_destroy(pal_helix_t* self);

// Does a deep copy of an helix.
//    dst: The destination helix.
//    src: The source helix.
#define pal_helix_cpy(dst,src) memcpy(dst, src, sizeof(pal_helix_t));

// Computes 'num' evenly spaced points.
void pal_helix_compute_points(const pal_helix_t* self, point2d_t* p, int num);

// Computes a single point at the parametric value 't'.
void pal_helix_compute_point(const pal_helix_t* self, point2d_t* p, double t);



// Initialize the curve test.
void pal_helix_init();

// De-initializes the curve test.
void pal_helix_deinit();

// Test whether this is a helix.
const pal_helix_result_t* pal_helix_test(const pal_stroke_t* stroke);

// Does a deep copy of a helix result.
//    dst: The destination helix result.
//    src: The source helix result.
static inline void
pal_helix_result_cpy(pal_helix_result_t* dst, const pal_helix_result_t* src) {
  memcpy(dst, src, sizeof(pal_helix_result_t));
}

#endif // __pal_helix_h__
