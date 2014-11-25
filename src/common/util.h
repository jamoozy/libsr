#ifndef __util_h__
#define __util_h__

#include <time.h>

#define swap(type, a, b) { type s = a; a = b; b = s; }

#define get_utime() ((long)time(NULL))

// Min and max copied from StackOverflow:
//   http://stackoverflow.com/questions/3437404/min-and-max-in-c

#define MAX(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a > _b ? _a : _b; })

#define MIN(a,b) \
   ({ __typeof__ (a) _a = (a); \
       __typeof__ (b) _b = (b); \
     _a < _b ? _a : _b; })

#endif  // __util_h__

