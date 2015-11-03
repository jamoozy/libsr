#ifndef __dollarp_h__
#define __dollarp_h__

#include <stdlib.h>

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
dp_context_t* dp_context_create();

/*! Sets the epsilon value for the context.
 *
 * \param self The context.
 * \param epsilon The new value.
 */
static inline void dp_set_epsilon(dp_context_t* self, double epsilon) {
  self->epsilon = epsilon;
  self->step = floor(pow(self->n, 1 - self->epsilon));
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
void destroy_dollarp(dp_context_t* self);

#endif
