#ifndef __util_h__
#define __util_h__

#include <config.h>
#include <math.h>
#include <time.h>

#define swap(type, a, b) { type s = a; a = b; b = s; }

#define get_utime() ((long)time(NULL))

// Min and max copied from the GNU manual:
//    https://gcc.gnu.org/onlinedocs/cpp/
//          Duplication-of-Side-Effects.html#Duplication-of-Side-Effects

#define MAX(X, Y)                \
({ __typeof__ (X) x_ = (X);          \
   __typeof__ (Y) y_ = (Y);          \
   (x_ > y_) ? x_ : y_; })

#define MIN(X, Y)                \
({ __typeof__ (X) x_ = (X);          \
   __typeof__ (Y) y_ = (Y);          \
   (x_ < y_) ? x_ : y_; })


// Normalizes an angle to be within [-pi,pi]

#define NORM_ANGLE(angle) do { \
  if (angle > M_PIl) { \
    angle += 2 * M_PIl; \
  } else if (angle < -M_PIl) { \
    angle -= 2 * M_PIl; \
  } \
} while (0)

#define SWAP(x,y) do { \
  __typeof__ (x) tmp = x; \
  x = y; \
  y = tmp; \
} while (0)


#endif  // __util_h__

