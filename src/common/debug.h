#ifndef __debug_h__
#define __debug_h__

#if DEBUG

#include <stdio.h>

//! Number of times `EN` has been called minus the number of times `EX` has been
//! called.  Used by `PRINT_INDENT`.
static int __debug_indent = 0;

//! Print the indent = (#`EN` - #`EX`) &times; 2 spaces.
#define PRINT_INDENT() do { \
  for (int i = 0; i < __debug_indent; i++) { \
    printf("  "); \
  } \
} while (0)

//! Log entry into a function.
#define EN(msg, ...) do { \
  __debug_indent++; \
  PRINT_INDENT(); \
  printf("-->"); \
  printf(msg, __VA_ARGS__); \
} while (0)

//! Log a debug message.
#define debug(msg, ...) printf(msg, __VA_ARGS__)

//! Log exit from a function.
#define EX(msg, ...) { \
  PRINT_INDENT(); \
  printf("<--"); \
  printf(msg, __VA_ARGS__); \
  __debug_indent--; \
}

#else  // if !DEBUG

#define PRINT_INDENT()
#define EN(msg, ...)
#define debug(msg, ...)
#define EX(msg, ...)

#endif  // DEBUG
#endif  // __debug_h__
