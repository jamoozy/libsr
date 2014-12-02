#include <check.h>

#include "point.h"
#include "geom.h"



//////////////////////////////////////////////////////////////////////////////
// ------------------------------ Vec Tests ------------------------------- //
//////////////////////////////////////////////////////////////////////////////

// Tests that ||<x1,y1> x <x2,y2>|| == e.
static inline void _test_vec_cross_prod(double e,
    double x1, double y1, double x2, double y2) {
  point2d_t a = { x1, y1 }, b = { x2, y2 };
  ck_assert(vec_cross_prod(&a, &b) == e);
}

START_TEST(c_vec_cross_prod) {
  _test_vec_cross_prod(0, 20, 40, 90, -40);
} END_TEST

START_TEST(c_vec_cross_prod_same) {
  _test_vec_cross_prod(0, 20, 40, 20, 40);
} END_TEST

START_TEST(c_vec_cross_prod_opp) {
  _test_vec_cross_prod(0, 40, 20, -40, -20);
} END_TEST


static inline void _test_vec_sub(double xe, double ye,
    double x1, double y1, double x2, double y2) {
  point2d_t a = { x1, y1 }, b = { x2, y2 }, e;
  vec_sub(&e, &a, &b);
  ck_assert(e.x == xe);
  ck_assert(e.y == ye);
}

START_TEST(c_vec_sub) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_vec_sub_same) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_vec_sub_opp) {
  ck_assert_msg(0, "not impl");
} END_TEST


////////////////////////////////////////////////////////////////////////////////
// ------------------------------- Area Tests ------------------------------- //
////////////////////////////////////////////////////////////////////////////////

static inline void _test_triangle_area(double e,
    double x1, double y1, double x2, double y2, double x3, double y3) {
  point2d_t a = { x1, y1 }, b = { x2, y2 }, c = { x3, y3 };
  ck_assert(geom_triangle_area(&a, &b, &c) == e);
}

START_TEST(c_triangle_area_0) {
  _test_triangle_area(0, 0,0, 0,0, 0,0);
} END_TEST

START_TEST(c_quad_area_crossing) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_quad_area_square) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_quad_area_deg) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_quad_area_ovlp) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_quad_area_normal) {
  ck_assert_msg(0, "not impl");
} END_TEST



static inline void _test_quad_area(double e,
    double x1, double y1, double x2, double y2,
    double x3, double y3, double x4, double y4) {
  point2d_t a = { x1, y1 }, b = { x2, y2 }, c = { x3, y3 }, d = { x4, y4 };
  ck_assert(geom_quad_area(&a, &b, &c, &d) == e);
}

START_TEST(c_quad_area_0) {
  _test_quad_area(0, 0,0, 0,0, 0,0, 0,0);
} END_TEST

START_TEST(c_triangle_area_big) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_triangle_area_small) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_triangle_area_iso) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_triangle_area_right) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_triangle_area_eq) {
  ck_assert_msg(0, "not impl");
} END_TEST



static inline void _test_segs_intersect(double e,
    double x1, double y1, double x2, double y2,
    double x3, double y3, double x4, double y4) {
  point2d_t a = { x1, y1 }, b = { x2, y2 }, c = { x3, y3 }, d = { x4, y4 };
  ck_assert(geom_segs_intersect(&a, &b, &c, &d) == e);
}

START_TEST(c_segs_intersect_parallel) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_segs_intersect_overlapping) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_segs_intersect_same) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_segs_intersect_cross) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_segs_intersect_cross_front) {
  ck_assert_msg(0, "not impl");
} END_TEST


////////////////////////////////////////////////////////////////////////////////
// ------------------------------ Entry Point ------------------------------- //
////////////////////////////////////////////////////////////////////////////////

static Suite* geom_suite() {
  Suite* suite = suite_create("geom");

  TCase* tc = tcase_create("vec cross");
  tcase_add_test(tc, c_vec_cross_prod);
  tcase_add_test(tc, c_vec_cross_prod_same);
  tcase_add_test(tc, c_vec_cross_prod_opp);
  suite_add_tcase(suite, tc);

  tc = tcase_create("vec sub");
  tcase_add_test(tc, c_vec_sub);
  tcase_add_test(tc, c_vec_sub_same);
  tcase_add_test(tc, c_vec_sub_opp);
  suite_add_tcase(suite, tc);

  tc = tcase_create("triangle area");
  tcase_add_test(tc, c_triangle_area_0);
  tcase_add_test(tc, c_triangle_area_big);
  tcase_add_test(tc, c_triangle_area_small);
  tcase_add_test(tc, c_triangle_area_iso);
  tcase_add_test(tc, c_triangle_area_right);
  tcase_add_test(tc, c_triangle_area_eq);
  suite_add_tcase(suite, tc);

  tc = tcase_create("quad area");
  tcase_add_test(tc, c_quad_area_0);
  tcase_add_test(tc, c_quad_area_crossing);
  tcase_add_test(tc, c_quad_area_square);
  tcase_add_test(tc, c_quad_area_deg);
  tcase_add_test(tc, c_quad_area_ovlp);
  tcase_add_test(tc, c_quad_area_normal);
  suite_add_tcase(suite, tc);

  tc = tcase_create("segs intersect");
  tcase_add_test(tc, c_segs_intersect_parallel);
  tcase_add_test(tc, c_segs_intersect_overlapping);
  tcase_add_test(tc, c_segs_intersect_same);
  tcase_add_test(tc, c_segs_intersect_cross);
  tcase_add_test(tc, c_segs_intersect_cross_front);
  suite_add_tcase(suite, tc);

  return suite;
}

int main() {
  int number_failed = 0;
  Suite* suite = geom_suite();
  SRunner* runner = srunner_create(suite);

  srunner_run_all(runner, CK_VERBOSE);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
