#include <check.h>
#include <values.h>

#include "point.h"
#include "stroke.h"
#include "geom.h"


//! Convenience macro.
#define ck_assert_double_eq(x,y) ck_assert(GEOM_EQ(x, y))



//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Point Tests ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

// ---- Creation and Destruction ----

START_TEST(c_point_create) {
  point_t* point = point_create();
  ck_assert(point != NULL);
  ck_assert_double_eq(point->x, 0);
  ck_assert_double_eq(point->y, 0);
  ck_assert_int_eq(point->t, -1);
  ck_assert_int_eq(point->i, -1);
  point_destroy(point);
} END_TEST

START_TEST(c_point_create_coords) {
  point_t* point = point_create_coords(-20, 40);
  ck_assert(point != NULL);
  ck_assert_double_eq(point->x, -20);
  ck_assert_double_eq(point->y,  40);
  ck_assert_int_eq(point->t, -1);
  ck_assert_int_eq(point->i, -1);
  point_destroy(point);
} END_TEST

START_TEST(c_point_create_timed) {
  point_t* point = point_create_timed(80, -80, LONG_MAX);
  ck_assert(point != NULL);
  ck_assert_double_eq(point->x,  80);
  ck_assert_double_eq(point->y, -80);
  ck_assert_int_eq(point->t, LONG_MAX);
  ck_assert_int_eq(point->i, -1);
  point_destroy(point);
} END_TEST

START_TEST(c_point_create_full) {
  point_t* point = point_create_full(480, 1200, LONG_MAX, 80);
  ck_assert(point != NULL);
  ck_assert_double_eq(point->x,  480);
  ck_assert_double_eq(point->y, 1200);
  ck_assert_int_eq(point->t, LONG_MAX);
  ck_assert_int_eq(point->i, 80);
  point_destroy(point);
} END_TEST


// ---- point2d_equal ----

START_TEST(c_point2d_equal_0) {
  point2d_t a = {0, 0}, b = {0, 0};
  ck_assert(point2d_equal(&a, &b));
} END_TEST

START_TEST(c_point2d_equal_diff) {
  point2d_t a = {10, 80}, b = {110, 40};
  ck_assert(!point2d_equal(&a, &b));
} END_TEST


// ---- point2d_bis ----
//! \todo


//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Stroke Tests ----------------------------- //
//////////////////////////////////////////////////////////////////////////////

START_TEST(c_stroke_create) {
  stroke_t* stroke = stroke_create(40);
  ck_assert(stroke != NULL);
  ck_assert_int_eq(stroke->num, 0);
  ck_assert_int_eq(stroke->size, 40);
  ck_assert(stroke->pts != NULL);
  for (int i = 0; i < stroke->size; i++) {
    ck_assert_int_eq(stroke->pts[i].x, 0);
    ck_assert_int_eq(stroke->pts[i].y, 0);
    ck_assert_int_eq(stroke->pts[i].t, 0);
    ck_assert_int_eq(stroke->pts[i].i, 0);
  }
  stroke_destroy(stroke);
} END_TEST

START_TEST(c_stroke_create_point2dts) {
  point2dt_t points[5] = {
    { .x =  4.0, .y =   8.0, .t =       12},
    { .x =  2.0, .y =   4.0, .t =        8},
    { .x = 12.0, .y =  13.0, .t =       14},
    { .x =  0.0, .y =  14.0, .t =       28},
    { .x = 99.0, .y = 180.0, .t = LONG_MAX}
  };
  stroke_t* stroke = stroke_create_point2dts(5, (point2dt_t*)points);
  ck_assert(stroke != NULL);
  ck_assert_int_eq(stroke->num, 5);
  ck_assert_int_eq(stroke->size, 5);
  ck_assert(stroke->pts != NULL);
  ck_assert((point2dt_t*)stroke->pts != points);
  for (int i = 0; i < stroke->size; i++) {
    ck_assert_int_eq(stroke->pts[i].x, points[i].x);
    ck_assert_int_eq(stroke->pts[i].y, points[i].y);
    ck_assert_int_eq(stroke->pts[i].t, points[i].t);
    ck_assert_int_eq(stroke->pts[i].i, i);
  }
  stroke_destroy(stroke);
} END_TEST



//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Entry Point ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

static inline Suite* stroke_suite() {
  Suite* suite = suite_create("stroke");

  TCase* tc = tcase_create("point");
  tcase_add_test(tc, c_point_create);
  tcase_add_test(tc, c_point_create_coords);
  tcase_add_test(tc, c_point_create_timed);
  tcase_add_test(tc, c_point_create_full);
  tcase_add_test(tc, c_point2d_equal_0);
  tcase_add_test(tc, c_point2d_equal_diff);
  suite_add_tcase(suite, tc);

  tc = tcase_create("stroke");
  tcase_add_test(tc, c_stroke_create);
  tcase_add_test(tc, c_stroke_create_point2dts);
  suite_add_tcase(suite, tc);

  return suite;
}

int main() {
  int number_failed = 0;
  Suite* suite = stroke_suite();
  SRunner* runner = srunner_create(suite);

  srunner_run_all(runner, CK_VERBOSE);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
