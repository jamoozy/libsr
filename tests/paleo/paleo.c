#include <check.h>

#include "paleo.h"



////////////////////////////////////////////////////////////////////////////////
// ----------------------------- Sanity Checks ------------------------------ //
////////////////////////////////////////////////////////////////////////////////

START_TEST(c_pal_types_indet_differs) {
  ck_assert(PAL_TYPE_INDET != PAL_TYPE_UNRUN);
  ck_assert(PAL_TYPE_INDET != PAL_TYPE_DOT);
  ck_assert(PAL_TYPE_INDET != PAL_TYPE_LINE);
  ck_assert(PAL_TYPE_INDET != PAL_TYPE_CIRCLE);
  ck_assert(PAL_TYPE_INDET != PAL_TYPE_ELLIPSE);
  ck_assert(PAL_TYPE_INDET != PAL_TYPE_ARC);
  ck_assert(PAL_TYPE_INDET != PAL_TYPE_CURVE);
  ck_assert(PAL_TYPE_INDET != PAL_TYPE_SPIRAL);
  ck_assert(PAL_TYPE_INDET != PAL_TYPE_HELIX);
  ck_assert(PAL_TYPE_INDET != PAL_TYPE_COMPOSITE);
  ck_assert(PAL_TYPE_INDET != PAL_TYPE_NUM);
} END_TEST

START_TEST(c_pal_types_unrun_differs) {
  ck_assert(PAL_TYPE_UNRUN != PAL_TYPE_INDET);
  ck_assert(PAL_TYPE_UNRUN != PAL_TYPE_DOT);
  ck_assert(PAL_TYPE_UNRUN != PAL_TYPE_LINE);
  ck_assert(PAL_TYPE_UNRUN != PAL_TYPE_CIRCLE);
  ck_assert(PAL_TYPE_UNRUN != PAL_TYPE_ELLIPSE);
  ck_assert(PAL_TYPE_UNRUN != PAL_TYPE_ARC);
  ck_assert(PAL_TYPE_UNRUN != PAL_TYPE_CURVE);
  ck_assert(PAL_TYPE_UNRUN != PAL_TYPE_SPIRAL);
  ck_assert(PAL_TYPE_UNRUN != PAL_TYPE_HELIX);
  ck_assert(PAL_TYPE_UNRUN != PAL_TYPE_COMPOSITE);
  ck_assert(PAL_TYPE_UNRUN != PAL_TYPE_NUM);
} END_TEST

START_TEST(c_pal_types_gt_0) {
  ck_assert( 0 <= PAL_TYPE_DOT);
  ck_assert( 0 <= PAL_TYPE_LINE);
  ck_assert( 0 <= PAL_TYPE_CIRCLE);
  ck_assert( 0 <= PAL_TYPE_ELLIPSE);
  ck_assert( 0 <= PAL_TYPE_ARC);
  ck_assert( 0 <= PAL_TYPE_CURVE);
  ck_assert( 0 <= PAL_TYPE_SPIRAL);
  ck_assert( 0 <= PAL_TYPE_HELIX);
  ck_assert( 0 <= PAL_TYPE_COMPOSITE);
  ck_assert(10 == PAL_TYPE_NUM);
} END_TEST


////////////////////////////////////////////////////////////////////////////////
// ----------------------------- Simple Checks ------------------------------ //
////////////////////////////////////////////////////////////////////////////////

START_TEST(c_pal_init_deinit)
{ // Just making sure things don't break out of the gate.
  pal_init();
  ck_assert(PAL_TYPE_UNRUN == pal_last_type());
  pal_deinit();
}
END_TEST

START_TEST(c_pal_recognize_empty)
{
  pal_init();

  stroke_t stroke = { 0, 0, NULL };
  ck_assert(PAL_TYPE_INDET == pal_recognize(&stroke));

  pal_deinit();
}
END_TEST

START_TEST(c_pal_recognize_1_point)
{
  pal_init();

  stroke_t stroke = { 1, 1, calloc(1, sizeof(point_t)) };
  ck_assert(PAL_TYPE_INDET == pal_recognize(&stroke));
  ck_assert(PAL_TYPE_INDET == pal_last_type());

  pal_deinit();
}
END_TEST

START_TEST(c_pal_recognize_40_points)
{
  pal_init();

  stroke_t stroke = { 40, 40, calloc(40, sizeof(point_t)) };
  ck_assert(PAL_TYPE_INDET == pal_recognize(&stroke));
  ck_assert(PAL_TYPE_INDET == pal_last_type());

  pal_deinit();
}
END_TEST




//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Entry Point ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

static Suite* pal_suite() {
  Suite* suite = suite_create("paleo");

  TCase* tc = tcase_create("process");
  tcase_add_test(tc, c_pal_init_deinit);
  tcase_add_test(tc, c_pal_recognize_empty);
  tcase_add_test(tc, c_pal_recognize_1_point);
  tcase_add_test(tc, c_pal_recognize_40_points);
  suite_add_tcase(suite, tc);

  tc = tcase_create("sanity");
  tcase_add_test(tc, c_pal_types_indet_differs);
  tcase_add_test(tc, c_pal_types_unrun_differs);
  tcase_add_test(tc, c_pal_types_gt_0);
  suite_add_tcase(suite, tc);

  return suite;
}

int main() {
  int number_failed = 0;
  Suite* suite = pal_suite();
  SRunner* runner = srunner_create(suite);

  srunner_run_all(runner, CK_VERBOSE);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
