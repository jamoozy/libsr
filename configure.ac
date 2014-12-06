# Prelude
AC_INIT([libsr], [0.1.0], [libsr-bugs@jamoozy.com])
AC_CONFIG_AUX_DIR([.build-aux])
AC_SUBST([CFLAGS], ['-g -O2 -std=c99 -I$(top_srcdir)/src'])

AM_INIT_AUTOMAKE([-Wall -Werror])

# Checks for programs.
AM_PROG_AR
AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_RANLIB

# Checks for libraries.
AC_CHECK_LIB([m],[cos])
AC_CHECK_LIB([gslcblas],[cblas_dgemm])
AC_CHECK_LIB([gsl],[gsl_dgemm])
AX_PATH_GSL([1.16], [], [AC_MSG_ERROR([Require minimum GSL v1.16.])])
PKG_CHECK_MODULES([CHECK], [check >= 0.9], [],
                  [Warning, Check not installed, so tests not run.])

# Checks for header files
AC_CHECK_HEADERS([assert.h string.h strings.h stdlib.h stdio.h math.h values.h])
AC_DEFINE(_GNU_SOURCE, 1, [Define for math.h M_PI (and others) usage.])

# Checks for typedefs, structures, and compiler characteristics.
AC_C_INLINE
AC_TYPE_SIZE_T

# Checks for library functions.

# Output files.
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile src/paleo/Makefile src/common/Makefile
                 tests/Makefile tests/paleo/Makefile tests/common/Makefile])
AC_OUTPUT
