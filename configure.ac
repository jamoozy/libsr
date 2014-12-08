# Prelude
AC_INIT([libsr], [0.1.0], [libsr-bugs@jamoozy.com])
AC_CONFIG_AUX_DIR([.build-aux])
AC_SUBST([CFLAGS], ['-g -O2 -I$(top_srcdir)/src'])

AM_INIT_AUTOMAKE([-Wall -Werror])

# Checks for programs.
AM_PROG_AR
AC_PROG_CC
AC_PROG_CC_C_O
AC_PROG_CC_C99
AC_PROG_RANLIB

AC_C_CONST
AC_C_INLINE
AC_C_TYPEOF

# Checks for libraries.
AC_SEARCH_LIBS([sqrt], [m], [], AC_MSG_ERROR([No math library found.]))
AC_SEARCH_LIBS([LAPACKE_dgetri], [lapacke], [], AC_MSG_ERROR([Lapacke (C version) not found.]))
PKG_CHECK_MODULES([CHECK], [check >= 0.9], [],
                  [Warning, Check not installed, so tests not run.])

# Checks for header files
AC_CHECK_HEADERS([assert.h lapacke.h math.h string.h strings.h stdlib.h stdio.h values.h])
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
