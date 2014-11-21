#ifndef __util_h__
#define __util_h__

#include <time.h>

#define swap(type, a, b) { type s = a; a = b; b = s; }

#define get_utime() ((long)time(NULL))

#endif  // __util_h__

