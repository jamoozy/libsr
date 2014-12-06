#ifndef __util_h__
#define __util_h__

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


#endif  // __util_h__

