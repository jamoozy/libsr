#ifndef __debug_h__
#define __debug_h__

#ifdef DEBUG

#include <stdio.h>

//! Number of times `EN` has been called minus the number of times `EX` has been
//! called.  Used by `PRINT_INDENT`.
static int __debug_indent = 0;

//! Print the indent = (#`EN` - #`EX`) &times; 2 spaces.
#define PRINT_INDENT() do { \
  for (int i = 0; i < __debug_indent; i++) { \
    fprintf(stderr, "  "); \
  } \
} while (0)

//! Log entry into a function.
#define EN(msg, ...) do { \
  PRINT_INDENT(); \
  fprintf(stderr, "-->"); \
  fprintf(stderr, msg, ##__VA_ARGS__); \
  __debug_indent++; \
} while (0)

//! Log a debug message.
#define debug(msg, ...) do { \
  PRINT_INDENT(); \
  fprintf(stderr, msg, ##__VA_ARGS__); \
} while (0)

//! Log exit from a function.
#define EX(msg, ...) { \
  __debug_indent--; \
  PRINT_INDENT(); \
  fprintf(stderr, "<--"); \
  fprintf(stderr, msg, ##__VA_ARGS__); \
}

#else  // if !DEBUG

#define PRINT_INDENT()
#define EN(msg, ...)
#define debug(msg, ...)
#define EX(msg, ...)

#endif  // DEBUG
#endif  // __debug_h__
