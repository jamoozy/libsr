#include <assert.h>
#include <string.h>

#include "mock_point.h"
#include "common/point.h"

void mock_point_set(point_t* point) {
  mock_point = calloc(1, sizeof(mock_point_t));
  memcpy(mock_point, point, sizeof(point_t));
  mock_point->state = STATE_UNDEF;
}

// Mock function conforming to point API

point_t* point_create() {
  assert(mock_point != NULL);
  mock_point->state = STATE_ALLOC;
  return &(mock_point->point);
}

inline point_t* point_create_coords(long x, long y) {
  return point_create();
}

inline point_t* point_create_timed(long x, long y, long t) {
  return point_create();
}

inline point_t* point_create_full(long x, long y, long t, long i) {
  return point_create();
}

inline void point_destroy(point_t* point) {
  assert(&(mock_point->point) == point);
  mock_point->state = STATE_FREED;
}

#undef STATE_UNDEF
#undef STATE_ALLOC
#undef STATE_FREED
