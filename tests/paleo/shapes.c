#include <values.h>
#include <check.h>

#include "common/mock_point.h"
#include "line.h"
#include "ellipse.h"
#include "arc.h"
#include "curve.h"
#include "spiral.h"
#include "helix.h"
#include "complex.h"



//////////////////////////////////////////////////////////////////////////////
// ------------------------------ Line Tests ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

START_TEST(c_pal_line_create)
{
  pal_line_t* line = pal_line_create();
  ck_assert(line != NULL);
  ck_assert_int_eq(line->num, 2);
  ck_assert(line->pts != NULL);
  pal_line_destroy(line);
}
END_TEST

START_TEST(c_pal_line_create_points_with_points)
{
  point_t a = {.x = 5, .y = 10, .t = 20, .i = 40};
  point_t b = {.x =12, .y = 24, .t = 36, .i = 48};
  pal_line_t* line = pal_line_create_points_with_points(
      (point2d_t*)&a, (point2d_t*)&b);

  ck_assert(line != NULL);
  ck_assert_int_eq(line->num, 2);

  ck_assert(line->pts != NULL);
  ck_assert_int_eq(line->pts[0].x,  5);
  ck_assert_int_eq(line->pts[0].y, 10);
  ck_assert_int_eq(line->pts[1].x, 12);
  ck_assert_int_eq(line->pts[1].y, 24);

  pal_line_destroy(line);
}
END_TEST

START_TEST(c_pal_line_create_points_with_longs)
{
  pal_line_t* line = pal_line_create_points_with_longs(-20, 20, -40, 40);

  ck_assert(line != NULL);
  ck_assert_int_eq(line->num, 2);

  ck_assert(line->pts != NULL);
  ck_assert_int_eq(line->pts[0].x, -20);
  ck_assert_int_eq(line->pts[0].y,  20);
  ck_assert_int_eq(line->pts[1].x, -40);
  ck_assert_int_eq(line->pts[1].y,  40);

  pal_line_destroy(line);
}
END_TEST



//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Circle Tests ----------------------------- //
//////////////////////////////////////////////////////////////////////////////

START_TEST(c_circle_create)
{
  pal_circle_t* circle = pal_circle_create();
  ck_assert(circle != NULL);
  ck_assert_int_eq(circle->r, 0);
  ck_assert_int_eq(circle->center.x, 0);
  ck_assert_int_eq(circle->center.y, 0);
}
END_TEST

START_TEST(c_circle_create_full)
{
  pal_circle_t* circle = pal_circle_create_full(40, 20, 80);
  ck_assert(circle != NULL);
  ck_assert_int_eq(circle->r, 40);
  ck_assert_int_eq(circle->center.x, 20);
  ck_assert_int_eq(circle->center.y, 80);
}
END_TEST

START_TEST(c_circle_create_with_point)
{
  point_t point = {.x = 4, .y = 8, .t = 32, .i = 49};
  pal_circle_t* circle = pal_circle_create_with_point(40, (point2d_t*)&point);

  ck_assert(circle != NULL);
  ck_assert_int_eq(circle->r, 40);
  ck_assert_int_eq(circle->center.x, point.x);
  ck_assert_int_eq(circle->center.y, point.y);
}
END_TEST



//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Curve Tests ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

START_TEST(c_curve_create)
{
  pal_curve_t* curve = pal_curve_create(4);
  ck_assert(curve != NULL);
  ck_assert_int_eq(curve->num, 4);
  ck_assert(curve->pts != NULL);
  for (int i = 0; i < 4; i++) {
    ck_assert(curve->pts[i].x == 0);
    ck_assert(curve->pts[i].y == 0);
  }
  pal_curve_destroy(curve);
}
END_TEST

START_TEST(c_curve_create_coords)
{
  long xs[] = { 4, 8, 12, 16, 28 };
  long ys[] = { 12, 24, 48, 62 };
  pal_curve_t* curve = pal_curve_create_coords(4, xs, ys);
  ck_assert(curve != NULL);
  ck_assert_int_eq(curve->num, 4);
  ck_assert(curve->pts != NULL);
  for (int i = 0; i < curve->num; i++) {
    ck_assert_int_eq(curve->pts[i].x, xs[i]);
    ck_assert_int_eq(curve->pts[i].y, ys[i]);
  }
  pal_curve_destroy(curve);
}
END_TEST

START_TEST(c_curve_create_points)
{
  long xs[] = { 4, 8, 12, 16, 28 };
  long ys[] = { 12, 24, 48, 62 };
  point2d_t points[] = {
    { xs[0], ys[0] },
    { xs[1], ys[1] },
    { xs[2], ys[2] },
    { xs[3], ys[3] }
  };

  pal_curve_t* curve = pal_curve_create_points(4, points);

  ck_assert(curve != NULL);
  ck_assert_int_eq(curve->num, 4);
  ck_assert(curve->pts != NULL);
  for (int i = 0; i < curve->num; i++) {
    ck_assert(curve->pts[i].x == xs[i]);
    ck_assert(curve->pts[i].y == ys[i]);
  }
  pal_curve_destroy(curve);
}
END_TEST



//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Entry Point ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

static Suite* shapes_suite() {
  Suite* suite = suite_create("shapes");

  TCase* tc = tcase_create("line");
  tcase_add_test(tc, c_pal_line_create);
  tcase_add_test(tc, c_pal_line_create_points_with_points);
  tcase_add_test(tc, c_pal_line_create_points_with_longs);
  suite_add_tcase(suite, tc);

  tc = tcase_create("circle");
  tcase_add_test(tc, c_circle_create);
  tcase_add_test(tc, c_circle_create_full);
  tcase_add_test(tc, c_circle_create_with_point);
  suite_add_tcase(suite, tc);

  tc = tcase_create("curve");
  tcase_add_test(tc, c_curve_create);
  tcase_add_test(tc, c_curve_create_coords);
  tcase_add_test(tc, c_curve_create_points);
  suite_add_tcase(suite, tc);

  return suite;
}

int main() {
  int number_failed = 0;
  Suite* suite = shapes_suite();
  SRunner* runner = srunner_create(suite);

  srunner_run_all(runner, CK_VERBOSE);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
