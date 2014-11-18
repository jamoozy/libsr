#ifndef __mock_point_h__
#define __mock_point_h__

#include <assert.h>

#include "common/point.h"

// States that a mock can be in.
#define STATE_UNDEF 0
#define STATE_ALLOC 1
#define STATE_FREED 2

typedef struct {
  point2d_t point2d;
  int state;
} mock_point2d_t;

typedef struct {
  point2dt_t point2dt;
  int state;
} mock_point2dt_t;

typedef struct {
  point_t point;
  int state;
} mock_point_t;

// Mock objects to be returned/freed
static mock_point2d_t* mock_point2d = NULL;
static mock_point2dt_t* mock_point2dt = NULL;
static mock_point_t* mock_point = NULL;

static inline void assert_freed() {
  if(mock_point2d != NULL) {
    assert(mock_point2d->state == STATE_FREED);
    free(mock_point2d);
  }
  if(mock_point2dt != NULL) {
    assert(mock_point2dt->state == STATE_FREED);
    free(mock_point2dt);
  }
  if(mock_point != NULL) {
    assert(mock_point->state == STATE_FREED);
    free(mock_point);
  }
}

#endif  // __mock_point_h__
