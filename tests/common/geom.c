#include <check.h>

#include "point.h"
#include "geom.h"



//////////////////////////////////////////////////////////////////////////////
// ------------------------------ Vec Tests ------------------------------- //
//////////////////////////////////////////////////////////////////////////////

/*!
 * Tests that \f$\|\langle x_1,y_1\rangle \times \langle x_2,y_2\rangle\|=e\f$.
 *
 * \param e The expected result.
 * \param x1 X-coordinate of vector 1.
 * \param y1 Y-coordinate of vector 1.
 * \param x2 X-coordinate of vector 2.
 * \param y2 Y-coordinate of vector 2.
 */
static inline void
_test_vec_cross_prod(double e, double x1, double y1, double x2, double y2) {
  point2d_t a = { x1, y1 }, b = { x2, y2 };
  double r = vec_cross_prod(&a, &b);
  ck_assert_msg(r == e, "Expected %.2f, got %.2f", e, r);
}

START_TEST(c_vec_cross_prod) {
  _test_vec_cross_prod(-4400, 20, 40, 90, -40);
} END_TEST

START_TEST(c_vec_cross_prod_same) {
  _test_vec_cross_prod(0, 20, 40, 20, 40);
} END_TEST

START_TEST(c_vec_cross_prod_opp) {
  _test_vec_cross_prod(0, 40, 20, -40, -20);
} END_TEST


// ---- Vector Subtraction ------------

/*!
 * Tests that \f$\langle x_e,y_e\rangle =
 * \langle x_1,y_1\rangle - \langle x_2,y_2\rangle\f$
 *
 * \param xe X-coordinate of expected result vector.
 * \param xe Y-coordinate of expected result vector.
 * \param x1 X-coordinate of vector 1.
 * \param x1 Y-coordinate of vector 1.
 * \param x2 X-coordinate of vector 2.
 * \param x2 Y-coordinate of vector 2.
 */
static inline void _test_vec_sub(double xe, double ye,
                                 double x1, double y1, double x2, double y2) {
  point2d_t a = { x1, y1 }, b = { x2, y2 }, r;
  vec_sub(&r, &a, &b);
  ck_assert_msg(r.x == xe, "Expected %.2f, got %.2f", xe, r.x);
  ck_assert_msg(r.y == ye, "Expected %.2f, got %.2f", ye, r.y);
}

START_TEST(c_vec_sub) {
  _test_vec_sub(60,50, 40,80, -20,30);
} END_TEST

START_TEST(c_vec_sub_same) {
  _test_vec_sub(0,0, 40,-40, 40,-40);
} END_TEST

START_TEST(c_vec_sub_opp) {
  ck_assert_msg(80,-40, -40,20, 40,-20);
} END_TEST


//////////////////////////////////////////////////////////////////////////////
// ------------------------------ Area Tests ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

/*!
 * Ensures that the triangle with the specified points has the expected area.
 *
 * \param e The expected area of the triangle.
 * \param x1 The X-coordinate of point 1 in the triangle.
 * \param y1 The Y-coordinate of point 1 in the triangle.
 * \param x2 The X-coordinate of point 2 in the triangle.
 * \param y2 The Y-coordinate of point 2 in the triangle.
 * \param x3 The X-coordinate of point 3 in the triangle.
 * \param y3 The Y-coordinate of point 3 in the triangle.
 */
static inline void _test_triangle_area(double e, double x1, double y1,
                                       double x2, double y2,
                                       double x3, double y3) {
  point2d_t a = { x1, y1 },
            b = { x2, y2 },
            c = { x3, y3 };
  double r = geom_triangle_area(&a, &b, &c);
  ck_assert_msg(GEOM_EQ_0(r - e), "Expected %.2f, got %.2f", e, r);
}

START_TEST(c_triangle_area_0) {
  _test_triangle_area(0, 0,0, 0,0, 0,0);
} END_TEST

START_TEST(c_triangle_area_big) {
  _test_triangle_area(5200, 40,40, 80,90, 200,500);
} END_TEST

START_TEST(c_triangle_area_small) {
  _test_triangle_area(93, 30,12, 27,30, 20,10);
} END_TEST

START_TEST(c_triangle_area_iso) {
  _test_triangle_area(1050, 20,10, 0,60, 50,40);
} END_TEST

START_TEST(c_triangle_area_right) {
  _test_triangle_area(650, 30,4, 82,29, 82,4);
} END_TEST

START_TEST(c_triangle_area_eq) {
  _test_triangle_area(426.95, 24,30, 29,61, 53.346787517318, 41.169872981078);
} END_TEST


// ---- Quad Area ------------

/*!
 * Ensures that the quadrilateral with the specified points has the expected
 * area.
 *
 * \param e The expected area of the quadrilateral.
 * \param x1 The X-coordinate of point 1 in the quadrilateral.
 * \param y1 The Y-coordinate of point 1 in the quadrilateral.
 * \param x2 The X-coordinate of point 2 in the quadrilateral.
 * \param y2 The Y-coordinate of point 2 in the quadrilateral.
 * \param x3 The X-coordinate of point 3 in the quadrilateral.
 * \param y3 The Y-coordinate of point 3 in the quadrilateral.
 * \param x4 The X-coordinate of point 4 in the quadrilateral.
 * \param y4 The Y-coordinate of point 4 in the quadrilateral.
 */
static inline void _test_quad_area(double e,
                                   double x1, double y1, double x2, double y2,
                                   double x3, double y3, double x4, double y4) {
  point2d_t a = { x1, y1 }, b = { x2, y2 }, c = { x3, y3 }, d = { x4, y4 };
  double r = geom_quad_area(&a, &b, &c, &d);
  ck_assert_msg(GEOM_EQ_0(r - e), "Expected %.2f, got %.2f", e, r);
}

START_TEST(c_quad_area_0) {
  _test_quad_area(0, 0,0, 0,0, 0,0, 0,0);
} END_TEST

START_TEST(c_quad_area_crossing) {
  _test_quad_area(400, 20,100, 60,60, 60,80, 20,80);
} END_TEST

START_TEST(c_quad_area_square) {
  _test_quad_area(400, 20,20, 20,40, 40,40, 40,20);
} END_TEST

//! This is a degenerate quadrilateral.  It's equivalent to a triangle because
//! three of its points line up to make a straight line.
START_TEST(c_quad_area_deg) {
  _test_quad_area(300, 10,40, 20,10, 30,40, 40,40);
} END_TEST

//! This is an overlapping quadrilateral -- 2 of its points are the same.
START_TEST(c_quad_area_ovlp) {
  _test_quad_area(650, 30,4, 82,29, 82,29, 82,4);
} END_TEST

START_TEST(c_quad_area_normal) {
  _test_quad_area(10 * 1000, -10,-10, -10,90, 90,90, 90,-10);
} END_TEST



//////////////////////////////////////////////////////////////////////////////
// ---------------------------- Line / Segment ---------------------------- //
//////////////////////////////////////////////////////////////////////////////

// ---- geom_seg_seg_intersect ------------

/*!
 * Tests whether the two line segments intersect.
 *
 * \param e Whether they should be found to intersect.
 * \param x1 The X-coordinate of one of line 1's endpoints.
 * \param y1 The Y-coordinate of one of line 1's endpoints.
 * \param x2 The X-coordinate of one of line 1's endpoints.
 * \param y2 The Y-coordinate of one of line 1's endpoints.
 * \param x3 The X-coordinate of one of line 2's endpoints.
 * \param y3 The Y-coordinate of one of line 2's endpoints.
 * \param x4 The X-coordinate of one of line 2's endpoints.
 * \param y4 The Y-coordinate of one of line 2's endpoints.
 */
static inline void _test_seg_seg_intersect(
    char e,
    double x1, double y1, double x2, double y2,
    double x3, double y3, double x4, double y4) {
  point2d_t a = { x1, y1 }, b = { x2, y2 }, c = { x3, y3 }, d = { x4, y4 };
  char r = geom_seg_seg_intersect(&a, &b, &c, &d);
  ck_assert_msg(!!(r) == !!(e), "Expected %.2f, got %.2f", e, r);
}

START_TEST(c_seg_seg_intersect_parallel) {
  _test_seg_seg_intersect(0, 40,60, 60,40, 65,35, 85,15);
} END_TEST

//! 2 segments on the line \f$y = 4x+10\f$ where \f$x\in{20,42,89,200}\f$.
START_TEST(c_seg_seg_intersect_overlapping) {
  _test_seg_seg_intersect(0, 20,90, 89,366, 42,178, 200,810);
} END_TEST

START_TEST(c_seg_seg_intersect_same) {
  _test_seg_seg_intersect(0, 89,42, 36,202, 89,42, 36,202);
} END_TEST

START_TEST(c_seg_seg_intersect_cross) {
  _test_seg_seg_intersect(1, 20,20, 80,80, 20,80, 80,20);
} END_TEST

START_TEST(c_seg_seg_intersect_cross_vert) {
  _test_seg_seg_intersect(1, 30,40, 30,20, 40,30, 20,30);
} END_TEST

START_TEST(c_seg_seg_intersect_colinear) {
  _test_seg_seg_intersect(0, 20,100, 40,100, 41,100, 51,100);
} END_TEST

START_TEST(c_seg_seg_intersect_just_touching) {
  _test_seg_seg_intersect(1, 20,100, 40,100, 40,100, 40,80);
} END_TEST


// ---- geom_seg_seg_intersection ------------

/*!
 * Tests whether the two line segments intersect.
 *
 * \param e The intersection point or `NULL` if the lines shouldn't intersect.
 * \param x1 The X-coordinate of one of line 1's endpoints.
 * \param y1 The Y-coordinate of one of line 1's endpoints.
 * \param x2 The X-coordinate of one of line 1's endpoints.
 * \param y2 The Y-coordinate of one of line 1's endpoints.
 * \param x3 The X-coordinate of one of line 2's endpoints.
 * \param y3 The Y-coordinate of one of line 2's endpoints.
 * \param x4 The X-coordinate of one of line 2's endpoints.
 * \param y4 The Y-coordinate of one of line 2's endpoints.
 */
static inline void _test_seg_seg_intersection(
    point2d_t* e,
    double x1, double y1, double x2, double y2,
    double x3, double y3, double x4, double y4) {
  point2d_t a = { x1, y1 }, b = { x2, y2 }, c = { x3, y3 }, d = { x4, y4 }, r;
  char i = geom_seg_seg_intersection(&r, &a, &b, &c, &d);
  ck_assert_msg(!(i) == !(e), "%s intersection.",
                e == NULL ? "Did not expect" : "Expected");
  if (e != NULL) {
    ck_assert_msg(GEOM_EQ(r.x, e->x) && GEOM_EQ(r.y, e->y),
                  "Expected %.2f, got %.2f", e, r);
  }
}

START_TEST(c_seg_seg_intersection_parallel) {
  _test_seg_seg_intersection(NULL, 40,60, 60,40, 65,35, 85,15);
} END_TEST

//! 2 segments on the line \f$y = 4x+10\f$ where \f$x\in{20,42,89,200}\f$.
START_TEST(c_seg_seg_intersection_overlapping) {
  _test_seg_seg_intersection(NULL, 20,90, 89,366, 42,178, 200,810);
} END_TEST

START_TEST(c_seg_seg_intersection_same) {
  _test_seg_seg_intersection(NULL, 89,42, 36,202, 89,42, 36,202);
} END_TEST

START_TEST(c_seg_seg_intersection_cross) {
  point2d_t e = {50, 50};
  _test_seg_seg_intersection(&e, 20,20, 80,80, 20,80, 80,20);
} END_TEST

START_TEST(c_seg_seg_intersection_cross_vert) {
  point2d_t e = {30, 30};
  _test_seg_seg_intersection(&e, 30,40, 30,20, 40,30, 20,30);
} END_TEST

START_TEST(c_seg_seg_intersection_colinear) {
  _test_seg_seg_intersection(NULL, 20,100, 40,100, 41,100, 51,100);
} END_TEST

START_TEST(c_seg_seg_intersection_just_touching) {
  point2d_t e = {40, 100};
  _test_seg_seg_intersection(&e, 20,100, 40,100, 40,100, 40,80);
} END_TEST

// ---- geom_seg_line_intersect ------------

/*!
 * Tests whether the line segment and line intersect.
 *
 * \param e Whether they should be found to intersect.
 * \param x1 The X-coordinate of one of the segment's endpoints.
 * \param y1 The Y-coordinate of one of the segment's endpoints.
 * \param x2 The X-coordinate of one of the segment's endpoints.
 * \param y2 The Y-coordinate of one of the segment's endpoints.
 * \param x3 The X-coordinate of one point on the line.
 * \param y3 The Y-coordinate of one point on the line.
 * \param x4 The X-coordinate of one point on the line.
 * \param y4 The Y-coordinate of one point on the line.
 */
static inline void _test_seg_line_intersect(
    char e,
    double x1, double y1, double x2, double y2,
    double x3, double y3, double x4, double y4) {
  point2d_t a = { x1, y1 }, b = { x2, y2 }, c = { x3, y3 }, d = { x4, y4 };
  char r = geom_seg_line_intersect(&a, &b, &c, &d);
  ck_assert_msg(r == e, "Expected %.2f, got %.2f", e, r);
}

//! \todo write `geom_seg_line_intersect` tests.


// ---- geom_seg_line_intersection ------------

/*!
 * Tests that the line segment and line intersect at the specified point.
 *
 * \param e Where the line and segment are expected to intersect.
 * \param x1 The X-coordinate of one of the segment's endpoints.
 * \param y1 The Y-coordinate of one of the segment's endpoints.
 * \param x2 The X-coordinate of one of the segment's endpoints.
 * \param y2 The Y-coordinate of one of the segment's endpoints.
 * \param x3 The X-coordinate of one point on the line.
 * \param y3 The Y-coordinate of one point on the line.
 * \param x4 The X-coordinate of one point on the line.
 * \param y4 The Y-coordinate of one point on the line.
 */
static inline void _test_seg_line_intersection(
    const point2d_t* e,
    double x1, double y1, double x2, double y2,
    double x3, double y3, double x4, double y4) {
  point2d_t isect = { -1, -1 };
  point2d_t a = { x1, y1 }, b = { x2, y2 }, c = { x3, y3 }, d = { x4, y4 };
  char r = geom_seg_line_intersection(&isect, &a, &b, &c, &d);
  ck_assert_msg((!r) == (!e), "Expected %.2f, got %.2f", e, r);
  if (e) {
    ck_assert_msg(memcmp(&isect, e, sizeof(point2d_t)),
          "Expected (%.2f,%.2f), got (%.2f,%.2f)",
          e->x, e->y, isect.x, isect.y);
  }
}

//! \todo write `geom_seg_line_intersection` tests.


// ---- geom_line_line_intersection ------------

/*!
 * Tests that the lines intersect at the specified point.
 *
 * \param e Where the line and segment are expected to intersect.
 * \param x1 The X-coordinate of one point on line #1.
 * \param y1 The Y-coordinate of one point on line #1.
 * \param x2 The X-coordinate of one point on line #1.
 * \param y2 The Y-coordinate of one point on line #1.
 * \param x3 The X-coordinate of one point on line #2.
 * \param y3 The Y-coordinate of one point on line #2.
 * \param x4 The X-coordinate of one point on line #2.
 * \param y4 The Y-coordinate of one point on line #2.
 */
static inline void _test_line_line_intersection(
    const point2d_t* e,
    double x1, double y1, double x2, double y2,
    double x3, double y3, double x4, double y4) {
  point2d_t isect = { -1, -1 };
  point2d_t a = { x1, y1 }, b = { x2, y2 }, c = { x3, y3 }, d = { x4, y4 };
  char r = geom_line_line_intersection(&isect, &a, &b, &c, &d);
  ck_assert_msg((!r) == (!e), "Expected %.2f, got %.2f", e, r);
  if (e) {
    ck_assert_msg(memcmp(&isect, e, sizeof(point2d_t)),
          "Expected (%.2f,%.2f), got (%.2f,%.2f)",
          e->x, e->y, isect.x, isect.y);
  }
}

//! \todo write `geom_line_line_intersection` tests.



//////////////////////////////////////////////////////////////////////////////
// ------------------------------ Auxiliary ------------------------------- //
//////////////////////////////////////////////////////////////////////////////




//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Entry Point ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

/*!
 * Builds the geometry test suite.
 *
 * \return The newly created geometry test suite.
 */
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

  tc = tcase_create("seg-seg intersect");
  tcase_add_test(tc, c_seg_seg_intersect_parallel);
  tcase_add_test(tc, c_seg_seg_intersect_overlapping);
  tcase_add_test(tc, c_seg_seg_intersect_same);
  tcase_add_test(tc, c_seg_seg_intersect_cross);
  tcase_add_test(tc, c_seg_seg_intersect_cross_vert);
  tcase_add_test(tc, c_seg_seg_intersect_colinear);
  tcase_add_test(tc, c_seg_seg_intersect_just_touching);
  suite_add_tcase(suite, tc);

  tc = tcase_create("seg-seg intersection");
  tcase_add_test(tc, c_seg_seg_intersection_parallel);
  tcase_add_test(tc, c_seg_seg_intersection_overlapping);
  tcase_add_test(tc, c_seg_seg_intersection_same);
  tcase_add_test(tc, c_seg_seg_intersection_cross);
  tcase_add_test(tc, c_seg_seg_intersection_cross_vert);
  tcase_add_test(tc, c_seg_seg_intersection_colinear);
  tcase_add_test(tc, c_seg_seg_intersection_just_touching);
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
