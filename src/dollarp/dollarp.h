#ifndef __dollarp_h__
#define __dollarp_h__

#include <stdlib.h>

#include "common/stroke.h"

//! Maximum allowable name for templates.
#define MAX_TMPL_NAME_LEN 100

//! A template -- used to recognize strokes.
typedef struct {
  stroke_t* strk;                   //!< The underlying stroke.
  char name[MAX_TMPL_NAME_LEN+1];   //!< Name of the template.
} template_t;

//! The main $P context.
typedef struct {
  size_t norm_size;   //!< Number of strokes to use in `normalize`.
  double epsilon;     //!< \in [0,1], controls # of tested alignments.

  template_t* tmpls;  //!< Templates to use.
  size_t num_tmpls;   //!< Number of templates.
} dp_context_t;

//! A result of calling dp_recognize.
typedef struct {
  template_t* tmpl;   //!< The template recognition.
  int score;          //!< The score of the recognition.
} dp_result_t;

#endif
