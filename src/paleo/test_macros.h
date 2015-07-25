/*! \file macros.h
 * Contains several convenience macros used across PaleoSketch's recognizers.
 *
 * \addtogroup pal
 * \{
 */

#ifndef __pal_test_macros_h__
#define __pal_test_macros_h__

#include <stdio.h>
#include <string.h>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////
//    These macros are to be used for convenience in Paleo test files.  They  //
//       assume you have a global "context" struct that contains a "result"   //
//            struct which inherits from Paleo results by including the       //
//                      PALEO_RESULT_UNION macro at its top.                  //
////////////////////////////////////////////////////////////////////////////////

/*! Resets the context for testing.  Assumes that the variable \c "context"
 * exists, is the current scope, and has both \c stroke and \c
 * stroke.result.possible fields.
 *
 * \param stroke The stroke to reset the context with.
 */
#define RESET(stroke) do {     \
  context.stroke = stroke;     \
  context.result.possible = 1; \
} while (0)

/*! Convenience macro to set up the context's result as having failed.
 *
 * \param msg The format string-formatted reason for failure.
 * \param ... The args to the format string.
 */
#define SET_FAIL(msg, ...) do { \
  const int room = strlen(msg) + 100; \
  context.result.fmsg = realloc( \
      context.result.fmsg, room * sizeof(char)); \
  if (room <= snprintf(context.result.fmsg, room, msg, ##__VA_ARGS__)) { \
    fprintf(stderr, "Wrote too many bytes."); \
    assert(0); \
  } \
  context.result.possible = 0; \
} while (0)

/*! Effectively "asserts" \c cond.  If \c cond is false, calls \c
 * CHECK_RTN_RESULT with \c msg and the variable arguments.
 *
 * \param cond The condition to check.
 * \param msg The failure reason.
 * \param ... The arguments to the fmt string.
 */
#define CHECK_RTN_RESULT(cond, msg, ...) do { \
  if (!(cond)) { \
    SET_FAIL(msg, ##__VA_ARGS__); \
    return &context.result; \
  } \
} while (0)

/*! Convenience macro to set the failure reason and return from the current
 * function.
 *
 * \param msg The fmt string-formatted failure reason.
 * \param ... The arguments to msg.
 *
 * \see SET_FAIL
 */
#define SET_FAIL_RTN(msg, ...) do { \
  SET_FAIL(msg, ##__VA_ARGS__); \
  return; \
} while (0)

#endif  // __pal_test_macros_h__

/*! \} */
