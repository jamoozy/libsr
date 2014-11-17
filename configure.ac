AC_INIT([libsr], [0.1.0], [libsr-bugs@jamoozy.com])

AC_CONFIG_AUX_DIR([.build-aux])
AC_SUBST([CFLAGS], ['-g -O2 -std=c99'])

AM_INIT_AUTOMAKE([-Wall -Werror])

AM_PROG_AR
AC_PROG_CC
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_RANLIB

AC_C_INLINE
AC_TYPE_SIZE_T

AC_CHECK_HEADERS([stdlib.h math.h stdio.h])
AC_CHECK_LIB([m],[cos])
AC_CHECK_LIB([gslcblas],[cblas_dgemm])
AX_PATH_GSL([1.16], [], [AC_MSG_ERROR([Require minimum GSL v1.16.])])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile src/Makefile src/paleo/Makefile
                 tests/Makefile tests/paleo/Makefile])

PKG_CHECK_MODULES([CHECK], [check >= 0.9], [],
                  [Warning, Check not installed, so tests not run.])

AC_OUTPUT
