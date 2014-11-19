#include <limits.h>
#include <check.h>

#include "common/mock_point.h"
#include "line.h"
#include "circle.h"
#include "curve.h"
//#include "helix.h"
//#include "spiral.h"



//////////////////////////////////////////////////////////////////////////////
// ------------------------------ Line Tests ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

START_TEST(c_line_create)
{
  line_t* line = line_create();
  ck_assert(line != NULL);
  ck_assert_int_eq(line->num, 2);
  ck_assert(line->pts != NULL);
  line_destroy(line);
}
END_TEST

START_TEST(c_line_create_points_with_points)
{
  point_t a = { 5, 10, 20, 40};
  point_t b = {12, 24, 36, 48};
  line_t* line = line_create_points_with_points((point2d_t*)&a, (point2d_t*)&b);

  ck_assert(line != NULL);
  ck_assert_int_eq(line->num, 2);

  ck_assert(line->pts != NULL);
  ck_assert_int_eq(line->pts[0].x,  5);
  ck_assert_int_eq(line->pts[0].y, 10);
  ck_assert_int_eq(line->pts[1].x, 12);
  ck_assert_int_eq(line->pts[1].y, 24);

  line_destroy(line);
}
END_TEST

START_TEST(c_line_create_points_with_longs)
{
  line_t* line = line_create_points_with_longs(-20, 20, -40, 40);

  ck_assert(line != NULL);
  ck_assert_int_eq(line->num, 2);

  ck_assert(line->pts != NULL);
  ck_assert_int_eq(line->pts[0].x, -20);
  ck_assert_int_eq(line->pts[0].y,  20);
  ck_assert_int_eq(line->pts[1].x, -40);
  ck_assert_int_eq(line->pts[1].y,  40);

  line_destroy(line);
}
END_TEST



//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Circle Tests ----------------------------- //
//////////////////////////////////////////////////////////////////////////////

START_TEST(c_circle_create)
{
  circle_t* circle = circle_create();
  ck_assert(circle != NULL);
  ck_assert_int_eq(circle->r, 0);
  ck_assert_int_eq(circle->c.x, 0);
  ck_assert_int_eq(circle->c.y, 0);
}
END_TEST

START_TEST(c_circle_create_full)
{
  circle_t* circle = circle_create_full(40, 20, 80);
  ck_assert(circle != NULL);
  ck_assert_int_eq(circle->r, 40);
  ck_assert_int_eq(circle->c.x, 20);
  ck_assert_int_eq(circle->c.y, 80);
}
END_TEST

START_TEST(c_circle_create_with_point)
{
  point_t point = { 4, 8, 32, 49};
  circle_t* circle = circle_create_with_point(40, (point2d_t*)&point);

  ck_assert(circle != NULL);
  ck_assert_int_eq(circle->r, 40);
  ck_assert_int_eq(circle->c.x, point.x);
  ck_assert_int_eq(circle->c.y, point.y);
}
END_TEST



//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Curve Tests ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

START_TEST(c_curve_create)
{
  curve_t* curve = curve_create(4);
  ck_assert(curve != NULL);
  ck_assert_int_eq(curve->num, 4);
  ck_assert(curve->pts != NULL);
  curve_destroy(curve);
}
END_TEST

START_TEST(c_curve_create_coords)
{
  long xs[] = { 4, 8, 12, 16, 28 };
  long ys[] = { 12, 24, 48, 62 };
  curve_t* curve = curve_create_coords(4, xs, ys);
  ck_assert(curve != NULL);
  ck_assert_int_eq(curve->num, 4);
  ck_assert(curve->pts != NULL);
  for (int i = 0; i < curve->num; i++) {
    ck_assert_int_eq(curve->pts[i].x, xs[i]);
    ck_assert_int_eq(curve->pts[i].y, ys[i]);
  }
  curve_destroy(curve);
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

  curve_t* curve = curve_create_points(4, points);

  ck_assert(curve != NULL);
  ck_assert_int_eq(curve->num, 4);
  ck_assert(curve->pts != NULL);
  for (int i = 0; i < curve->num; i++) {
    ck_assert(curve->pts[i].x == xs[i]);
    ck_assert(curve->pts[i].y == ys[i]);
  }
  curve_destroy(curve);
}
END_TEST





//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Entry Point ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

static Suite* shapes_suite() {
  Suite* suite = suite_create("shapes");

  TCase* tc = tcase_create("line");
  tcase_add_test(tc, c_line_create);
  tcase_add_test(tc, c_line_create_points_with_points);
  tcase_add_test(tc, c_line_create_points_with_longs);
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

  srunner_run_all(runner, CK_MINIMAL);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
