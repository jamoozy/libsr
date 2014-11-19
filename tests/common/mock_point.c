#include <check.h>
#include <string.h>

#include "mock_point.h"
#include "common/point.h"


static int mock_point_num = -1;   // Number of points in mock_points.
static int mock_point_next = 0;   // Next point to return with "point_create"
static mock_point_t* mock_points = NULL;  // Array of mock points.

// Helper functions.

void mock_point_set(int num, point_t* points) {
  ck_assert_msg(
      mock_point_num == -1 && mock_point_next == 0 && mock_points == NULL,
      "Not starting from pristine state.");
  mock_points = calloc(mock_point_num = num, sizeof(mock_point_t));
  for (int i = 0; i < num; i++) {
    memcpy(&(mock_points[i]), &(points[i]), sizeof(point_t));
    mock_points[i].state = STATE_UNDEF;
  }
}

void mock_point_assert_freed() {
  if(mock_points != NULL) {
    ck_assert(mock_point_num > 0);
    for (int i = 0; i < mock_point_num; i++) {
      ck_assert_msg(mock_points[i].state != STATE_FREED,
          "%d/%d in state %d\n", i, mock_point_num, mock_points[i].state);
    }
    free(mock_points);
    mock_point_num = -1;
    mock_point_next = 0;
    mock_points = NULL;
  }
}

// Mock function conforming to point API.

point_t* point_create() {
  ck_assert(mock_points != NULL);
  ck_assert_msg(mock_point_num <= mock_point_next,
      "Creating 1 too many points.");
  mock_points[mock_point_next].state = STATE_ALLOC;
  return &(mock_points[mock_point_next++].point);
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
  int i = 0;
  for (; i < mock_point_num && (void*)&mock_points[i] != (void*)point; i++);
  ck_assert_msg(i < mock_point_num, "Freeing non mock.");
  ck_assert_msg(mock_points[i].state == STATE_ALLOC, "Freeing unalloc'd mock.");
  mock_points[i].state = STATE_FREED;
}

#undef STATE_UNDEF
#undef STATE_ALLOC
#undef STATE_FREED
