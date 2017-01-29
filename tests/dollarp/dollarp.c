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

/*! Adds a template to a $P context.
 *
 * \param ctx The $P context.
 * \param fname The name of the template file to load.
 * \param name The name of the shape it contains.
 *
 * \return 0 on success, non-zero on failure of any kind.
 */
int _add_to_template(dp_context_t* ctx, const char* fname, const char* name) {
  stroke_t* strk = load_stroke(fname);
  if (strk == NULL) {
    return 1;
  }
  dp_add_template(ctx, strk, name);

  debug("Destroying local copy of stroke from %s: %p\n", fname, strk);
  stroke_destroy(strk);

  return 0;
}

START_TEST(c_dp_add_small_template) {
  dp_context_t* ctx = dp_create();

  // A 10-point stroke representing a circle.
  if (_add_to_template(ctx, "data/circle.stroke.srz", "fake stroke")) {
    dp_destroy(ctx);
    ck_abort_msg("Could not load stroke.");
  }

  debug("Destroying context: %p\n", ctx);
  dp_destroy(ctx);
} END_TEST

START_TEST(c_dp_add_many_large_templates) {
  dp_context_t* ctx = dp_create();

  // add several templates for a rectangle.
  const int num = 4;
  const char* fnames[] = {
    "data/rect1.stroke.srz",
    "data/rect2.stroke.srz",
    "data/rect3.stroke.srz",
    "data/rect4.stroke.srz"
  };
  for (int i = 0; i < num; i++) {
    if (_add_to_template(ctx, fnames[i], "rectangle")) {
      dp_destroy(ctx);
      ck_abort_msg("Could not load stroke.");
    }
  }

  debug("Destroying context: %p\n", ctx);
  dp_destroy(ctx);
} END_TEST



////////////////////////////////////////////////////////////////////////////////
// ---------------------------- Cross-Validation ---------------------------- //
////////////////////////////////////////////////////////////////////////////////

/*! Does a recognition of stroke.
 *
 * Asserts that the recognition is what was expected.
 *
 * \param ctx The $P context to use.
 * \param fname The name of the file to read in and test.
 * \param name The name of the template we're expecting.
 *
 * \return 0 on success, non-zero on an error of some kind.
 */
int _do_rec(const dp_context_t* ctx, const char* fname, const char* name) {
  stroke_t* strk = load_stroke(fname);
  if (strk == NULL) {
    return 1;
  }

  debug("Running test of stroke at %s\n", fname);
  dp_result_t res = dp_recognize(ctx, strk);
  debug("Destroying local copy of stroke from %s: %p\n", fname, strk);
  stroke_destroy(strk);

  if (res.tmpl == NULL) {
    return 2;
  }
  if (0 != strcmp(res.tmpl->name, name)) {
    return 3;
  }

  return 0;
}

START_TEST(c_dp_full_rect_test) {
  const int num = 4;
  const char* circle_fname = "data/circle.stroke.srz";
  const char* fnames[] = {
    "data/rect1.stroke.srz",
    "data/rect2.stroke.srz",
    "data/rect3.stroke.srz",
    "data/rect4.stroke.srz"
  };
  const char* shape_name = "rectangle";

  for (int i = 0; i < num; i++) {
    debug("Testing on case #%d\n", i);
    dp_context_t* ctx = dp_create();

    // Add a circle stroke so that we have the opportunity to fail.
    if (_add_to_template(ctx, circle_fname, "circle")) {
      dp_destroy(ctx);
      ck_abort_msg("Could not load circle stroke.");
    }

    // Add the training data.
    for (int j = 0; j < num; j++) {
      if (j == i) {
        continue;
      } else if (_add_to_template(ctx, fnames[i], shape_name)) {
        dp_destroy(ctx);
        ck_abort_msg("Could not load stroke.");
      }
    }

    // Run the test.
    int ret = _do_rec(ctx, fnames[i], shape_name);
    switch (ret) {
    case 1:
      dp_destroy(ctx);
      ck_abort_msg("Could not load stroke.");
    case 2:
      dp_destroy(ctx);
      ck_abort_msg("Got NULL template back.");
    case 3:
      dp_destroy(ctx);
      ck_abort_msg("Wrong result.");
    }

    debug("Destroying context: %p\n", ctx);
    dp_destroy(ctx);
  }
} END_TEST



////////////////////////////////////////////////////////////////////////////////
// ------------------------------ Entry Point ------------------------------- //
////////////////////////////////////////////////////////////////////////////////

static inline Suite* dp_suite() {
  Suite* suite = suite_create("stroke");

  TCase* tc = tcase_create("setters");
  tcase_add_test(tc, c_dp_create);
  tcase_add_test(tc, c_dp_set_n_epsilon_1);
  tcase_add_test(tc, c_dp_set_n_epsilon_2);
  tcase_add_test(tc, c_dp_set_n_epsilon_3);
  tcase_add_test(tc, c_dp_set_n_too_low_1);
  tcase_add_test(tc, c_dp_set_n_too_low_2);
  tcase_add_test(tc, c_dp_set_n_too_high_1);
  tcase_add_test(tc, c_dp_set_n_too_high_2);
  tcase_add_test(tc, c_dp_set_n_too_high_3);
  suite_add_tcase(suite, tc);

  tc = tcase_create("templates");
  tcase_add_test(tc, c_dp_add_small_template);
  tcase_add_test(tc, c_dp_add_many_large_templates);
  suite_add_tcase(suite, tc);

  tc = tcase_create("running");
  tcase_add_test(tc, c_dp_full_rect_test);
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
