#include <check.h>
#include <limits.h>

#include "common/geom.h"
#include "dollarp/dollarp.h"
#include "common/mock_stroke.h"



////////////////////////////////////////////////////////////////////////////////
//              Context Creation, Destruction, and Manipulation               //
////////////////////////////////////////////////////////////////////////////////

START_TEST(c_dp_create)
{
  dp_context_t* ctx = dp_create();
  ck_assert(ctx != NULL);
  ck_assert(DP_DEFAULT_N == ctx->n);
  ck_assert(DP_DEFAULT_EPSILON == ctx->epsilon);
  ck_assert(ctx->num == 0);
  dp_destroy(ctx);
}
END_TEST

START_TEST(c_dp_set_n_epsilon_1)
{
  dp_context_t* ctx = dp_create();
  dp_set_n(ctx, 32);
  dp_set_epsilon(ctx, 0.5);

  ck_assert(DP_DEFAULT_N == ctx->n);
  ck_assert(0.5 == ctx->n);
  ck_assert(GEOM_EQ(5.656854249492381, ctx->step));

  dp_destroy(ctx);
}
END_TEST

START_TEST(c_dp_set_n_epsilon_2)
{
  dp_context_t* ctx = dp_create();
  dp_set_n(ctx, 1279);
  dp_set_epsilon(ctx, 0.2);

  ck_assert(1279 == ctx->n);
  ck_assert(0.2 == ctx->n);
  ck_assert(GEOM_EQ(305.8415144851744, ctx->step));

  dp_destroy(ctx);
}
END_TEST

START_TEST(c_dp_set_n_epsilon_3)
{
  dp_context_t* ctx = dp_create();
  dp_set_n(ctx, 9);
  dp_set_epsilon(ctx, 0.8);

  ck_assert(DP_DEFAULT_N == ctx->n);
  ck_assert(0.5 == ctx->n);
  ck_assert(GEOM_EQ(1.5518455739153598, ctx->step));

  dp_destroy(ctx);
}
END_TEST

START_TEST(c_dp_set_n_too_low_1)
{
  dp_context_t* ctx = dp_create();
  ck_assert(dp_set_epsilon(ctx, -0.1) == -1);
  dp_destroy(ctx);
}
END_TEST

START_TEST(c_dp_set_n_too_low_2)
{
  dp_context_t* ctx = dp_create();
  ck_assert(dp_set_epsilon(ctx, -100000) == -1);
  dp_destroy(ctx);
}
END_TEST

START_TEST(c_dp_set_n_too_high_1)
{
  dp_context_t* ctx = dp_create();
  ck_assert(dp_set_epsilon(ctx, 1.001) == 1);
  dp_destroy(ctx);
}
END_TEST

START_TEST(c_dp_set_n_too_high_2)
{
  dp_context_t* ctx = dp_create();
  ck_assert(dp_set_epsilon(ctx, 2) == -1);
  dp_destroy(ctx);
}
END_TEST

START_TEST(c_dp_set_n_too_high_3)
{
  dp_context_t* ctx = dp_create();
  ck_assert(dp_set_epsilon(ctx, 2872) == -1);
  dp_destroy(ctx);
}
END_TEST



////////////////////////////////////////////////////////////////////////////////
//                                 Templates                                  //
////////////////////////////////////////////////////////////////////////////////

START_TEST(c_dp_add_template)
{
  // Random 10-point stroke.
  stroke_t* strk = mock_stroke(10, 20, 20, 1234,
                                   40, 40, 1274,
                                   80, 80, 1274,
                                   40, 70, 1774,
                                   48, 70, 2274,
                                   20, 20, 3234,
                                   40, 40, 7474,
                                   80, 80, 8274,
                                   90, 70, 8474,
                                   28, 60, 9074);
  dp_context_t* ctx = dp_create();
  dp_add_template(ctx, strk, "fake stroke");
  dp_destroy(ctx);
  stroke_destroy(strk);
}
END_TEST

//START_TEST()
//{
//}
//END_TEST


//////////////////////////////////////////////////////////////////////////////
// ----------------------------- Entry Point ------------------------------ //
//////////////////////////////////////////////////////////////////////////////

static Suite* dp_suite() {
  Suite* suite = suite_create("stroke");

  TCase* tc = tcase_create("point");
  tcase_add_test(tc, c_dp_create);
  tcase_add_test(tc, c_dp_set_n_epsilon_1);
  tcase_add_test(tc, c_dp_set_n_epsilon_2);
  tcase_add_test(tc, c_dp_set_n_epsilon_3);
  tcase_add_test(tc, c_dp_set_n_too_low_1);
  tcase_add_test(tc, c_dp_set_n_too_low_2);
  tcase_add_test(tc, c_dp_set_n_too_high_1);
  tcase_add_test(tc, c_dp_set_n_too_high_2);
  tcase_add_test(tc, c_dp_set_n_too_high_3);
  tcase_add_test(tc, c_dp_add_template);
  suite_add_tcase(suite, tc);

  return suite;
}

int main() {
  int number_failed = 0;
  Suite* suite = dp_suite();
  SRunner* runner = srunner_create(suite);

  srunner_run_all(runner, CK_VERBOSE);
  number_failed = srunner_ntests_failed(runner);
  srunner_free(runner);

  return (number_failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
