#include <check.h>
#include <errno.h>
#include <libgen.h>
#include <unistd.h>
#include <values.h>

#define DEBUG
#include "common/debug.h"
#include "common/geom.h"
#include "common/mock_stroke.h"
#include "dollarp/dollarp.h"


#define PS "/" // Path Separator

#define BUF_SZ 1024
stroke_t* load_stroke(const char* fname) {
  char buf[BUF_SZ];

  debug("__FILE__: %s\n", __FILE__);
  const char* cwd = getcwd(buf, BUF_SZ);
  if (cwd == NULL) {
    const char* errstr = strerror(errno);
    fprintf(stderr, "Could not getcwd: %s\n", errstr);
    return NULL;
  }
  debug("cwd: %s\n", cwd);
  debug("fname: %s\n", fname);

  size_t sz = strlen(cwd) + strlen(fname) + 4;
  char* path = calloc(sz, sizeof(char));

  int act = snprintf(path, sz, "%s"PS"%s", cwd, fname);
  debug("path: %s\n", path);
  if (act > sz) {
    fprintf(stderr, "Error: %zd extra chars not written.\n", act - sz);
    return NULL;
  }
  return stroke_from_file(path);

//  stroke_t* strk = NULL;
//  char* full = realpath(path, buf);
//  if (full == NULL) {
//    const char* errstr = strerror(errno);
//    fprintf(stderr, "Could not get realpath: %s\n", errstr);
//  } else {
//    debug("full: %s\n", full);
//    strk = stroke_from_file(full);
//  }
//  free(path);
//
//  return strk;
}
#undef BUF_SZ



////////////////////////////////////////////////////////////////////////////////
//              Context Creation, Destruction, and Manipulation               //
////////////////////////////////////////////////////////////////////////////////

START_TEST(c_dp_create) {
  dp_context_t* ctx = dp_create();
  ck_assert(ctx != NULL);
  ck_assert(DP_DEFAULT_N == ctx->n);
  ck_assert(DP_DEFAULT_EPSILON == ctx->epsilon);
  ck_assert(ctx->num == 0);
  dp_destroy(ctx);
} END_TEST

START_TEST(c_dp_set_n_epsilon_1) {
  dp_context_t* ctx = dp_create();
  dp_set_n(ctx, 32);
  dp_set_epsilon(ctx, 0.5);

  ck_assert(32 == ctx->n);
  ck_assert(0.5 == ctx->epsilon);
  ck_assert(GEOM_EQ(5.656854249492381, ctx->step));

  dp_destroy(ctx);
} END_TEST

START_TEST(c_dp_set_n_epsilon_2) {
  dp_context_t* ctx = dp_create();
  dp_set_n(ctx, 1279);
  dp_set_epsilon(ctx, 0.2);

  ck_assert(1279 == ctx->n);
  ck_assert(0.2 == ctx->epsilon);
  ck_assert(GEOM_EQ(305.8415144851744, ctx->step));

  dp_destroy(ctx);
} END_TEST

START_TEST(c_dp_set_n_epsilon_3) {
  dp_context_t* ctx = dp_create();
  dp_set_n(ctx, 9);
  dp_set_epsilon(ctx, 0.8);

  ck_assert(9 == ctx->n);
  ck_assert(0.8 == ctx->epsilon);
  ck_assert(GEOM_EQ(1.5518455739153598, ctx->step));

  dp_destroy(ctx);
} END_TEST

START_TEST(c_dp_set_n_too_low_1) {
  dp_context_t* ctx = dp_create();
  ck_assert(dp_set_epsilon(ctx, -0.1) == -1);
  dp_destroy(ctx);
} END_TEST

START_TEST(c_dp_set_n_too_low_2) {
  dp_context_t* ctx = dp_create();
  ck_assert(dp_set_epsilon(ctx, -100000) == -1);
  dp_destroy(ctx);
} END_TEST

START_TEST(c_dp_set_n_too_high_1) {
  dp_context_t* ctx = dp_create();
  ck_assert(dp_set_epsilon(ctx, 1.001) == 1);
  dp_destroy(ctx);
} END_TEST

START_TEST(c_dp_set_n_too_high_2) {
  dp_context_t* ctx = dp_create();
  ck_assert(dp_set_epsilon(ctx, 2) == 1);
  dp_destroy(ctx);
} END_TEST

START_TEST(c_dp_set_n_too_high_3) {
  dp_context_t* ctx = dp_create();
  ck_assert(dp_set_epsilon(ctx, 2872) == 1);
  dp_destroy(ctx);
} END_TEST



////////////////////////////////////////////////////////////////////////////////
//                                 Templates                                  //
////////////////////////////////////////////////////////////////////////////////

START_TEST(c_dp_add_template) {
  // Random 10-point stroke.
  stroke_t* strk = load_stroke("data/circle.stroke.srz");
  if (strk == NULL) {
    ck_abort_msg("Could not load stroke.");
    return;
  }
  dp_context_t* ctx = dp_create();
  dp_add_template(ctx, strk, "fake stroke");

  debug("Destroying local copy of stroke: %p\n", strk);
  stroke_destroy(strk);

  debug("Destroying context: %p\n", ctx);
  dp_destroy(ctx);
} END_TEST



////////////////////////////////////////////////////////////////////////////////
// ------------------------------ Entry Point ------------------------------- //
////////////////////////////////////////////////////////////////////////////////

static inline Suite* dp_suite() {
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
