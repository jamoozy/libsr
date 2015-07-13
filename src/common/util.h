/*! @file util.h
 * A collection of several generally useful utility function.
 */

#ifndef __util_h__
#define __util_h__

#include <config.h>
#include <math.h>
#include <time.h>

//! Gets the current time.
#define get_utime() ((long)time(NULL))

// Min and max copied from the GNU manual:
//    https://gcc.gnu.org/onlinedocs/cpp/
//          Duplication-of-Side-Effects.html#Duplication-of-Side-Effects

//! Gets the maximum of `X` and `Y`.
#define MAX(X, Y)           \
({ __typeof__ (X) x_ = (X); \
   __typeof__ (Y) y_ = (Y); \
   (x_ > y_) ? x_ : y_; })

//! Gets the minimum of `X` and `Y`.
#define MIN(X, Y)           \
({ __typeof__ (X) x_ = (X); \
   __typeof__ (Y) y_ = (Y); \
   (x_ < y_) ? x_ : y_; })


//! Normalizes `angle` to be within \f$[-\pi,\pi]\f$.
#define NORM_ANGLE(angle) do { \
  if (angle > M_PIl) { \
    angle += 2 * M_PIl; \
  } else if (angle < -M_PIl) { \
    angle -= 2 * M_PIl; \
  } \
} while (0)

//! Swaps the values/references of `x` and `y`.
#define SWAP(x,y) do { \
  __typeof__ (x) tmp = x; \
  x = y; \
  y = tmp; \
} while (0)


//! Find the sign of a number:
#define SGN(X)              \
({ __typeof__ (X) x_ = (X); \
   x_ / abs(x_); })

#endif  // __util_h__

