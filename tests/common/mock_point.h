#ifndef __mock_point_h__
#define __mock_point_h__

#include <assert.h>

#include "common/point.h"

// States that a mock can be in.
#define STATE_UNDEF 0
#define STATE_ALLOC 1
#define STATE_FREED 2

//typedef struct {
//  point2d_t point2d;
//  int state;
//} mock_point2d_t;
//
//typedef struct {
//  point2dt_t point2dt;
//  int state;
//} mock_point2dt_t;

typedef struct {
  point_t point;
  int state;
} mock_point_t;

// Mock objects to be returned/freed
//static int mock_point2d_num = -1;
//static mock_point2d_t* mock_point2d = NULL;
//
//static int mock_point2dt_num = -1;
//static mock_point2dt_t* mock_point2dt = NULL;

// Sets the points to be returned by calls to forms of point_create().  The
// points are returned in the order of the array.
//   num: The number of points.
//   points: The array of points.
void mock_point_set(int num, point_t* points);

// Asserts that all the created points were freed, frees them, and resets the
// state of the mocks.
void mock_point_assert_freed();

#endif  // __mock_point_h__
