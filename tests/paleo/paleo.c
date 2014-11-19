#include <check.h>

#include "paleo.h"


START_TEST(c_paleo_init_deinit)
{ // Just making sure things don't break out of the gate.
  paleo_init();
  ck_assert(PALEO_TYPE_UNRUN == paleo_last_type());
  paleo_deinit();
}
END_TEST

START_TEST(c_paleo_recognize_empty)
{
  paleo_init();

  stroke_t stroke = { 0, 0, NULL };
  ck_assert(PALEO_TYPE_INDET == paleo_recognize(&stroke));

  paleo_deinit();
}
END_TEST

START_TEST(c_paleo_recognize_1_point)
{
  paleo_init();

  stroke_t stroke = { 1, 1, calloc(1, sizeof(point_t)) };
  ck_assert(PALEO_TYPE_INDET == paleo_recognize(&stroke));
  ck_assert(PALEO_TYPE_INDET == paleo_last_type());

  paleo_deinit();
}
END_TEST

START_TEST(c_paleo_recognize_40_points)
{
  paleo_init();

  stroke_t stroke = { 40, 40, calloc(40, sizeof(point_t)) };
  ck_assert(PALEO_TYPE_INDET == paleo_recognize(&stroke));
  ck_assert(PALEO_TYPE_INDET == paleo_last_type());

  paleo_deinit();
}
END_TEST




//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Entry Point ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

static Suite* paleo_suite() {
  Suite* suite = suite_create("paleo");

  TCase* tc = tcase_create("process");
  tcase_add_test(tc, c_paleo_init_deinit);
  tcase_add_test(tc, c_paleo_recognize_empty);
  tcase_add_test(tc, c_paleo_recognize_1_point);
  tcase_add_test(tc, c_paleo_recognize_40_points);
  suite_add_tcase(suite, tc);

  return suite;
}

int main() {
  int number_failed = 0;
  Suite* suite = paleo_suite();
  SRunner* runner = srunner_create(suite);

  srunner_run_all(runner, CK_VERBOSE);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
