#ifndef __util_h__
#define __util_h__

#include <time.h>

inline long get_utime() {
  time_t t = time(NULL);
  return (long)t;
}

#endif  // __util_h__
