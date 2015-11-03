#ifndef __dollarp_h__
#define __dollarp_h__

#include <stdlib.h>
#include <stdio.h>

#include "common/stroke.h"

//! Maximum allowable name for templates.
#define DP_MAX_TMPL_NAME_LEN 100

//! A template -- used to recognize strokes.
typedef struct {
  stroke_t* strk;                     //!< The underlying stroke.
  char name[DP_MAX_TMPL_NAME_LEN+1];  //!< Name of the template.
} dp_template_t;

//! Default value for `dp_context_t.n`.
#define DP_DEFAULT_N 32

//! Default value for `dp_context_t.epsilon`.
#define DP_DEFAULT_EPSILON 0.50

/*! The main $P context.
 *
 * This structure holds the templates and some heuristic values.  Feel free to
 * alter `n`, and `epsilon`, but DO NOT mess with `tmpls`, `num`, or `cap`.
 */
typedef struct {
  size_t n;               //!< Number of strokes to use in `normalize`.
  double epsilon;         //!< \in [0,1], controls # of tested alignments.
  double step;            //!< Step used for scanning a stroke.

  dp_template_t* tmpls;   //!< Array of templates to use.
  size_t num;             //!< Number of templates.
  size_t cap;             //!< Capacity of the `tmpls` array.
} dp_context_t;

//! A result of calling dp_recognize.
typedef struct {
  dp_template_t* tmpl;    //!< The template recognition.
  double score;           //!< The score of the recognition.
} dp_result_t;


/*! Creates a new $P context.
 * \return The context.
 */
dp_context_t* dp_create();

/*! Sets the number of points in a normalized template/stroke.  Must be greater
 * than 0.
 *
 * \param self The $P context.
 * \param n The number of points to set per template/stroke.
 *
 * \return 0 on success, 1 if `n` is too low.
 */
static inline int dp_set_n(dp_context_t* self, size_t n) {
  if (n <= 0) {
    fprintf(stderr, "Error: dp_set_n called with n = %zd", n);
    return 1;
  }

  self->n = n;
  self->step = round(pow(self->n, 1 - self->epsilon));
  return 0;
}

/*! Sets the epsilon value for the context.  Must be \f$\in [0,1]\f$.
 *
 * \param self The context.
 * \param epsilon The new value.
 *
 * \return 0 on success, -1 if `epsilon` is too low, and 1 if it's too high.
 */
static inline int dp_set_epsilon(dp_context_t* self, double epsilon) {
  if (epsilon < 0.0) {
    fprintf(stderr, "Error: dp_set_epsilon called with epsilon = %f", epsilon);
    return -1;
  } else if (1.0 < epsilon) {
    fprintf(stderr, "Error: dp_set_epsilon called with epsilon = %f", epsilon);
    return 1;
  }

  self->epsilon = epsilon;
  self->step = round(pow(self->n, 1 - self->epsilon));
  return 0;
}

/*! Adds a new template to the $P context.  The $P context takes ownership of
 * the stroke and will most likely mutate it.
 *
 * Note that the maximum length of stroke names are limited to
 * `DP_MAX_TMPL_NAME_LEN`.  Names will be truncated to this length.
 *
 * \param strk The stroke the template is based on.
 * \param name The name of the template.
 */
void dp_add_template(dp_context_t* self, stroke_t* strk, const char* name);

/*! Attempts to recognize the stroke as one of the templates previously added to
 * the context with `db_add_template`.
 *
 * \return The result of the recognition; may be `NULL`.
 */
dp_result_t dp_recognize(const dp_context_t* self, stroke_t* strk);

/*! Destroys the $P context and frees all its memory
 *
 * \param self The $P context to delete.
 */
void dp_destroy(dp_context_t* self);

#endif
