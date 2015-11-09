#include <check.h>

#include "common/mock_point.h"
#include "line.h"
#include "ellipse.h"
#include "arc.h"
#include "curve.h"
#include "spiral.h"
#include "helix.h"
#include "complex.h"



// Testing Strategy:
//  1. Test "simple" result copying for ...
//    a. Line (2 pts) (possible)
//    b. Polyline (3,4,8 pts) (possible/not)
//    c. Circle/Ellipse (possible)
//    d. Arc (possible)
//    e. Curve (possible)
//    f. Spiral (not possible)
//    g. Helix (not possible)
//    h. A simple composite with a-g above.
//    i. A complex composite with a&d
//    j. A complex composite with f&g


// General test for 2 results inheriting from pal_result_t.
#define CK_ASSERT_PAL_RES_EQ(res,cpy) do { \
  ck_assert_msg(res.lse == cpy.lse, \
      "LSE's differ: %.2f != %.2f", res.lse, cpy.lse); \
  ck_assert_msg(res.fa == cpy.fa, \
      "FA's differ: %.2f != %.2f", res.fa, cpy.fa); \
  ck_assert_msg(strcmp(res.fmsg, cpy.fmsg) == 0, \
      "fmsg's differ: %s != %s", res.fmsg, cpy.fmsg); \
  ck_assert_msg(res.fmsg != cpy.fmsg, "fmsg's have same address."); \
  ck_assert_msg(res.possible == cpy.possible, \
      "\"possible\" different: %d vs.  %d.", res.possible, cpy.possible); \
} while (0)



//////////////////////////////////////////////////////////////////////////////
// ------------------------- Result Copying Tests ------------------------- //
//////////////////////////////////////////////////////////////////////////////

// Creates a pal_line_result_cpy check test.
//   name: Name of the test.
//   def: Definition of the result to copy and check.
#define LINE_RESULT_CPY_TEST(name,def) START_TEST(name) { \
} END_TEST

// ---- 1a ------------

//LINE_RESULT_CPY_TEST(c_line_result_cpy_possible,
//    );

START_TEST(c_line_result_cpy_possible) {
  // Keep it on the stack!
  pal_line_sub_result_t sub_res = {
    .lse = 40.09,
    .fa = -2.1,
    .fmsg = "success",
    .possible = 1,
    .line = { 0, NULL }
  };
  pal_line_result_t res = { &sub_res, 1 };

  // Set up memory for test.
  pal_line_sub_result_t sub_res_cpy;
  pal_line_result_t res_cpy;
  bzero(&sub_res_cpy, sizeof(pal_line_sub_result_t));
  bzero(&res_cpy, sizeof(pal_line_result_t));
  res_cpy.res = &sub_res_cpy;

  pal_line_result_cpy(&res_cpy, &res);

  ck_assert_msg(res.num == res_cpy.num,
      "num differs: %d vs. %d", res.num, res_cpy.num);

  for (int i = 0; i < res.num; i++) {
    CK_ASSERT_PAL_RES_EQ(res.res[i], res_cpy.res[i]);
    ck_assert_msg(res.res[i].line.num == res_cpy.res[i].line.num,
        "num differs: %d vs. %d", res.num, res_cpy.num);
  }
} END_TEST

// ---- 1b ------------

START_TEST(c_pline_result_cpy_3_pts_not) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_pline_result_cpy_4_pts_possible) {
  ck_assert_msg(0, "not impl");
} END_TEST

START_TEST(c_pline_result_cpy_8_pts_not) {
  ck_assert_msg(0, "not impl");
} END_TEST



//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Entry Point ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

static Suite* recs_suite() {
  Suite* suite = suite_create("recognition");

  TCase* tc = tcase_create("line");
  tcase_add_test(tc, c_line_result_cpy_possible);
  tcase_add_test(tc, c_pline_result_cpy_3_pts_not);
  tcase_add_test(tc, c_pline_result_cpy_4_pts_possible);
  tcase_add_test(tc, c_pline_result_cpy_8_pts_not);
  suite_add_tcase(suite, tc);

  tc = tcase_create("circle");
  suite_add_tcase(suite, tc);

  tc = tcase_create("ellipse");
  suite_add_tcase(suite, tc);

  tc = tcase_create("arc");
  suite_add_tcase(suite, tc);

  tc = tcase_create("curve");
  suite_add_tcase(suite, tc);

  tc = tcase_create("spiral");
  suite_add_tcase(suite, tc);

  tc = tcase_create("helix");
  suite_add_tcase(suite, tc);

  tc = tcase_create("complex");
  suite_add_tcase(suite, tc);

  return suite;
}

int main() {
  int number_failed = 0;
  Suite* suite = recs_suite();
  SRunner* runner = srunner_create(suite);

  srunner_run_all(runner, CK_VERBOSE);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
