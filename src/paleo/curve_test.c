#include <config.h>
#include <string.h>
#include <strings.h>

#if HAVE_OPENBLAS_CBLAS_H
# include <openblas/cblas.h>
#elif HAVE_CBLAS_H
# include <cblas.h>
#else
# error No blas API!
#endif

#if HAVE_LAPACKE_H
# include <lapacke.h>
#else
# error Could not find Lapack!
#endif

#include "common/point.h"
#include "test_macros.h"
#include "curve_test.h"



// The curve test context.
static curve_test_context_t context;

void curve_test_init() { bzero(&context, sizeof(curve_test_context_t)); }

void curve_test_deinit() { }

void    _reset(const paleo_stroke_t* stroke) {
  bzero(&context, sizeof(curve_test_result_t));
  context.stroke = stroke;
  context.result.possible = 1;
}


// The inverse of the M4 matrix (see docs/bezier.pdf).
static const double M4_INV[] = {
  0, 0, 0, 1, 0, 0, 1.0/3, 1, 0, 1.0/3, 2.0/3, 1, 1, 1, 1, 1
};

// The inverse of the M5 matrix (see docs/bezier.pdf).
static const double M5_INV[] = {
  0, 0, 0, 0, 1,
  0, 0, 0, .25, 1,
  0, 0, 1.0/6.0, .5, 1,
  0, .25, .5, .75, 1,
  1, 1, 1, 1, 1
};



////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Curve Test ------------------------------- //
////////////////////////////////////////////////////////////////////////////////

// Attempt to fit the stroke to a Bezier curve of degree d.
void _fit(const double* m_inv, const int d);

// Compute the squared distance between two points (for square error
// calculation).
static inline double _sq_err(const point2d_t a, const point2d_t b) {
  double dx = b.x-a.x;
  double dy = b.y-a.y;
  return dx*dx+dy*dy;
}

const curve_test_result_t* curve_test(const paleo_stroke_t* stroke) {
  CHECK_RTN_RESULT(stroke->dcr < PALEO_THRESH_J,
      "DCR too high: %.2f >= %.2f", stroke->dcr, PALEO_THRESH_J);

  _reset(stroke);

  // Pre-compute matrices common to any degree curve:
  context.Xs = calloc(stroke->num_pts, sizeof(double));
  context.Ys = calloc(stroke->num_pts, sizeof(double));
  for (int i = 0; i < stroke->num_pts; i++) {
    point2d_t* p = &stroke->pts[i].p2d;
    context.Xs[i] = p->x;
    context.Ys[i] = p->y;
  }

  // Find 2 different solutions (one for 4 control points and one for 5) by
  // using the LSE method described here:
  //    http://jimherold.com/2012/04/20/least-squares-bezier-fit/
  _fit(M4_INV, 4);
  _fit(M5_INV, 5);

  free(context.Xs);
  free(context.Ys);

  // Compute the LSE for each type of curve.
  context.ideal_4.lse = _sq_err(context.ideal_4.Cs[0], stroke->pts[0].p2d);
  context.ideal_5.lse = _sq_err(context.ideal_5.Cs[0], stroke->pts[0].p2d);
  double px_len = 0;
  for (int i = 1; i < stroke->num_pts; i++) {
    px_len += point2d_distance(&stroke->pts[i-1].p2d, &stroke->pts[i].p2d);
    double t = px_len / stroke->px_length;

    // Compute the B_4, then the B_5 point.
    point2d_t a;
    curve_util_compute_point(&a, context.ideal_4.Cs, 4, t);
    context.ideal_4.lse += _sq_err(stroke->pts[i].p2d, a);
    curve_util_compute_point(&a, context.ideal_5.Cs, 5, t);
    context.ideal_5.lse += _sq_err(stroke->pts[i].p2d, a);
  }

  if (context.ideal_4.lse < PALEO_THRESH_R) {
    context.result.curve.num = 4;
    memcpy(context.result.curve.pts, context.ideal_4.Cs,
        4 * sizeof(point2d_t));
    return &context.result;
  }

  CHECK_RTN_RESULT(context.ideal_5.lse < PALEO_THRESH_R,
      "LSE's too high: 4 (%.2f) & 5 (%.2f) >= %.2f",
      context.ideal_4.lse, context.ideal_5.lse, PALEO_THRESH_R);

  context.result.curve.num = 5;
  memcpy(context.result.curve.pts, context.ideal_5.Cs,
      5 * sizeof(point2d_t));
  return &context.result;
}

// Finds the inverse of the nxn matrix m, m_inv.
int _inv(double* m_inv, const double* m, int n);

// Multiplies the 2 nxn matrices 'a' and 'b' and puts the result into 'c'.
// 'a' is assumed to be nxk and 'b' is assumed to be kxm, so 'c' will be nxm.
static inline void _mul(double* c, const double* a, const double* b, int m, int k, int n);

void _fit(const double* m_inv, const int d) {
  assert(d == 4 || d == 5);   // XXX Current assumption.

  const int NP = context.stroke->num_pts;

  // Compute T & T^T.
  double px_len = 0;
  double* T = calloc(NP * d, sizeof(double));
  double* T_T = calloc(NP * d, sizeof(double));   // T^T
  for (int i = 0; i < NP; i++) {
    if (i != 0) {
      px_len += point2d_distance(
          &context.stroke->pts[i-1].p2d, &context.stroke->pts[i].p2d);
    }

    double t = px_len / context.stroke->px_length;
    T[i+(d-1)*NP] = T_T[(d-1)+i*NP] = 1;
    for (int j = d-2; j >= 0; j--) {
      T[i+j*NP] = T_T[j+i*NP] = T[i+(j+1)*NP] * t;
    }
  }

#ifndef NDEBUG
  // Sanity check that T and T_T are actually each others' transposes.
  for (int i = 0; i < NP; i++) {
    for (int j = 0; j < d; j++) {
      assert(T[i+j*NP] == T_T[j+i*NP]);
    }
  }
#endif

  // First compute matrix common to both the X and Y operations.

  // T^T T
  double* TTT = calloc(d*d, sizeof(double));
  _mul(TTT, T_T, T, d, NP, d);

  // (T^T T)^-1
  double* TTT_inv = calloc(d*d, sizeof(double));
  int rtn = _inv(TTT_inv, TTT, d);
  if (rtn != 0) {
    fprintf(stderr, "Could not invert matrix:\n");
    for (int i = 0; i < d; i++) {
      fprintf(stderr, "  [ %3.2f", TTT[i]);
      for (int j = 1; j < d; j++) {
        fprintf(stderr, ", %3.2f", TTT[i+j*d]);
      }
      fprintf(stderr, " ]\n");
    }
  }

  // (T^T T)^-1 T^T
  double* TTT_inv_TT = calloc(d*d, sizeof(double));
  _mul(TTT_inv_TT, TTT_inv, T_T, d, d, d);

  // M^-1 (T^T T)^-1 T^T
  double* M_inv_TTT_inv_TT = calloc(d*d, sizeof(double));
  _mul(M_inv_TTT_inv_TT, m_inv, TTT_inv, d, d, d);

  // Find control points' X and Y coords.
  double* C_x = calloc(d, sizeof(double));
  _mul(C_x, M_inv_TTT_inv_TT, context.Xs, d, d, 1);
  double* C_y = calloc(d, sizeof(double));
  _mul(C_y, M_inv_TTT_inv_TT, context.Ys, d, d, 1);

  // Convert to point2d_t's.
  point2d_t* cs = (d == 4 ? context.ideal_4.Cs : context.ideal_5.Cs);
  for (int i = 0; i < d; i++) {
    cs[i].x = C_x[i];
    cs[i].y = C_y[i];
  }

  free(C_y);
  free(C_x);
  free(M_inv_TTT_inv_TT);
  free(TTT_inv_TT);
  free(TTT_inv);
  free(TTT);
  free(T_T);
  free(T);
}

int _inv(double* m_inv, const double* m, int n) {
  memcpy(m_inv, m, n * n * sizeof(double));
  int* ipiv = calloc(n, sizeof(double));

  int rtn = LAPACKE_dgetrf(LAPACK_COL_MAJOR, n, n, m_inv, n, ipiv);
  if (rtn < 0) {
    fprintf(stderr, "%dth arg to dgetrf is illegal", -rtn);
  } else if (rtn > 0) {
    // For A = P * L * U
    fprintf(stderr, "U(%d,%d) is 0", rtn, rtn);
  } else {
    rtn = LAPACKE_dgetri(LAPACK_COL_MAJOR, n, m_inv, n, ipiv);
    assert(rtn <= 0);
    if (rtn < 0) {
      fprintf(stderr, "%dth arg to dgetri is illegal", -rtn);
    }
  }

  free(ipiv);
  return rtn;
}

static inline void _mul(double* c, const double* a, const double* b, int m, int k, int n) {
  return cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans,
      m, n, k, 1, a, m, b, k, 0, c, m);
}
