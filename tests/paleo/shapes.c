#include <check.h>

#include "point.h"
#include "line.h"
#include "circle.h"
#include "curve.h"
//#include "helix.h"
//#include "spiral.h"



//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Point Tests ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

START_TEST(c_point_create)
{
  point_t* point = point_create();
  ck_assert(point != NULL);
  ck_assert_int_eq(point->x, 0);
  ck_assert_int_eq(point->y, 0);
  ck_assert_int_eq(point->t, -1);
  ck_assert_int_eq(point->i, -1);
  point_destroy(point);
}
END_TEST

START_TEST(c_point_create_coords)
{
  point_t* point = point_create_coords(-20, 40);
  ck_assert(point != NULL);
  ck_assert_int_eq(point->x, -20);
  ck_assert_int_eq(point->y,  40);
  ck_assert_int_eq(point->t, -1);
  ck_assert_int_eq(point->i, -1);
  point_destroy(point);
}
END_TEST

START_TEST(c_point_create_timed)
{
  point_t* point = point_create_timed(80, -80, 109828365);
  ck_assert(point != NULL);
  ck_assert_int_eq(point->x,  80);
  ck_assert_int_eq(point->y, -80);
  ck_assert_int_eq(point->t, 109828365);
  ck_assert_int_eq(point->i, -1);
  point_destroy(point);
}
END_TEST

START_TEST(c_point_create_full)
{
  point_t* point = point_create_full(480, 1200, 9109828365, 80);
  ck_assert(point != NULL);
  ck_assert_int_eq(point->x,  480);
  ck_assert_int_eq(point->y, 1200);
  ck_assert_int_eq(point->t, 9109828365);
  ck_assert_int_eq(point->i, 80);
  point_destroy(point);
}
END_TEST



//////////////////////////////////////////////////////////////////////////////
// ------------------------------ Line Tests ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

START_TEST(c_line_create)
{
  line_t* line = line_create();
  ck_assert(line != NULL);
  ck_assert_int_eq(line->num, 2);
  ck_assert(line->points != NULL);
  line_destroy(line);
}
END_TEST

START_TEST(c_line_create_points_with_points)
{
  point_t* a = point_create_full( 5, 10, 20, 40);
  point_t* b = point_create_full(12, 24, 36, 48);
  line_t* line = line_create_points_with_points(a, b);

  ck_assert(line != NULL);
  ck_assert_int_eq(line->num, 2);
  ck_assert(line->points != NULL);

  ck_assert(line->points[0] == a);
  ck_assert_int_eq(line->points[0]->x,  5);
  ck_assert_int_eq(line->points[0]->y, 10);
  ck_assert_int_eq(line->points[0]->t, 20);
  ck_assert_int_eq(line->points[0]->i, 40);

  ck_assert(line->points[1] == b);
  ck_assert_int_eq(line->points[1]->x, 12);
  ck_assert_int_eq(line->points[1]->y, 24);
  ck_assert_int_eq(line->points[1]->t, 36);
  ck_assert_int_eq(line->points[1]->i, 48);

  line_destroy(line);
}
END_TEST

START_TEST(c_line_create_points_with_longs)
{
  line_t* line = line_create_points_with_longs(-20, 20, -40, 40);

  ck_assert(line != NULL);
  ck_assert_int_eq(line->num, 2);
  ck_assert(line->points != NULL);

  ck_assert(line->points[0] != NULL);
  ck_assert_int_eq(line->points[0]->x, -20);
  ck_assert_int_eq(line->points[0]->y,  20);

  ck_assert(line->points[1] != NULL);
  ck_assert_int_eq(line->points[1]->x, -40);
  ck_assert_int_eq(line->points[1]->y,  40);

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
  ck_assert(circle->c != NULL);
}
END_TEST

START_TEST(c_circle_create_full)
{
  circle_t* circle = circle_create_full(40, 20, 80);

  ck_assert(circle != NULL);
  ck_assert_int_eq(circle->r, 40);
  ck_assert(circle->c != NULL);
  ck_assert_int_eq(circle->c->x, 20);
  ck_assert_int_eq(circle->c->y, 80);
}
END_TEST

START_TEST(c_circle_create_with_point)
{
  point_t* point = point_create();
  circle_t* circle = circle_create_with_point(40, point);

  ck_assert(circle != NULL);
  ck_assert_int_eq(circle->r, 40);
  ck_assert(circle->c == point);
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
  ck_assert(curve->points != NULL);
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
  ck_assert(curve->points != NULL);
  for (int i = 0; i < curve->num; i++) {
    ck_assert(curve->points[i] != NULL);
    ck_assert_int_eq(curve->points[i]->x, xs[i]);
    ck_assert_int_eq(curve->points[i]->y, ys[i]);
  }
  curve_destroy(curve);
}
END_TEST

START_TEST(c_curve_create_points)
{
  long xs[] = { 4, 8, 12, 16, 28 };
  long ys[] = { 12, 24, 48, 62 };
  point_t* points[] = {
    point_create_coords(xs[0], ys[0]),
    point_create_coords(xs[1], ys[1]),
    point_create_coords(xs[2], ys[2]),
    point_create_coords(xs[3], ys[3])
  };
  curve_t* curve = curve_create_points(4, points);
  ck_assert(curve != NULL);
  ck_assert_int_eq(curve->num, 4);
  ck_assert(curve->points != NULL);
  ck_assert(curve->points != points);
  for (int i = 0; i < curve->num; i++) {
    ck_assert(curve->points[i] == points[i]);
    ck_assert_int_eq(curve->points[i]->x, xs[i]);
    ck_assert_int_eq(curve->points[i]->y, ys[i]);
  }
  curve_destroy(curve);
}
END_TEST





//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Entry Point ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

Suite* shapes_suite() {
  Suite* suite = suite_create("shapes");

  TCase* tc = tcase_create("point");
  tcase_add_test(tc, c_point_create);
  tcase_add_test(tc, c_point_create_coords);
  tcase_add_test(tc, c_point_create_timed);
  tcase_add_test(tc, c_point_create_full);
  suite_add_tcase(suite, tc);

  tc = tcase_create("line");
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

  srunner_run_all(runner, CK_NORMAL);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
