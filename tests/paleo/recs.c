

//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Entry Point ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

static Suite* recs_suite() {
  Suite* suite = suite_create("recognition");

  TCase* tc = tcase_create("line");
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
