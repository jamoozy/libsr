# Prelude
AC_PREREQ(2.69)
AC_INIT([libsr], [0.1.0], [libsr-bugs@jamoozy.com])
AC_CONFIG_AUX_DIR([.build-aux])
AC_SUBST([CFLAGS], ['-Wall -g -O2 -I$(top_srcdir)/src'])

AM_INIT_AUTOMAKE([-Wall -Werror])

# Checks for typedefs, structures, and compiler characteristics.
AM_PROG_AR
AC_PROG_CC
AM_PROG_CC_C_O  # complains on 32-bit?
AC_PROG_CC_C_O
AC_PROG_CC_C99
LT_INIT
AC_PROG_LIBTOOL
AC_CONFIG_MACRO_DIR([m4])
ACLOCAL_AMFLAGS+=-I m4

AC_C_CONST
AC_C_INLINE
AC_C_TYPEOF

# Check for LaTeX stuff.
AC_PATH_PROG(PDFLATEX, [pdflatex])
AS_IF([test "x$PDFLATEX" = x], [have_pdflatex="no"], [have_pdflatex="yes"])
AM_CONDITIONAL([HAVE_PDFLATEX], test "x$PDFLATEX" = "xyes")

AC_PATH_PROG(LATEX, [latex])
AC_PATH_PROG(DVIPDF, [dvipdf])
AS_IF([test "x$LATEX" = x -o "x$DVIPDF" = x],
  [have_latex="no"], [have_latex="yes"])
AM_CONDITIONAL([HAVE_LATEX], test "x$LATEX" = "xyes" -a "xDVIPDF" = "xyes")

AS_IF([test "x$have_pdflatex" = "xyes" -o "x$have_latex" = "xyes"],
  [have_any_latex="yes"], [have_any_latex="no"])
AM_CONDITIONAL([HAVE_ANY_LATEX],
  test "x$HAVE_LATEX_DVIPDF" = "xyes" -o "x$HAVE_PDFLATEX" = "xyes")

AC_TYPE_SIZE_T

# Checks for libraries.
AC_SEARCH_LIBS([sqrt], [m], [], [
  AC_MSG_ERROR([No math library found.])
])
AC_SEARCH_LIBS([cblas_dgemm], [openblas cblas blas], [], [
  AC_MSG_ERROR([OpenBLAS/BLAS not found])
])
AC_SEARCH_LIBS([LAPACKE_dgetri], [lapacke lapack], [], [
  AC_MSG_ERROR([Lapacke (C version) not found.])
])
PKG_CHECK_MODULES([CHECK], [check >= 0.9], [have_check="yes"], [
  AC_MSG_WARN([Check not installed, so tests not run.])
])
AM_CONDITIONAL([HAVE_CHECK], test "x$have_check" = "xyes")

# Checks for standard library functions and headers.
AC_CHECK_HEADERS(
  assert.h check.h cblas.h lapacke.h limits.h math.h openblas/cblas.h Python.h \
  string.h strings.h stdlib.h stdio.h values.h)
AC_CHECK_FUNCS_ONCE([abs assert bzero memcpy memmove floor sqrt atan sin cos])

# We're using GNU's stuff!
AC_DEFINE(_GNU_SOURCE, 1, [Define for math.h M_PI (and others) usage.])

# Check for bindings dependencies.
AC_PATH_PROG(SWIG, [swig])
AM_CONDITIONAL([HAVE_SWIG], test -n "$SWIG")
AS_IF([test -n "$SWIG"], [
  AC_MSG_NOTICE("swig found.  Bindings will be built.")

  # Python-specific stuff.
  AC_PATH_PROG(PYTHON, [python])
  AX_PYTHON_DEVEL([>= '2.7'])
  AM_CONDITIONAL([HAVE_PYTHON_DEV], test -n "$PYTHON_CPPFLAGS")
  AS_IF([test -n "$PYTHON_CPPFLAGS"],
    AS_IF([test -n "$SWIG"],
      AC_MSG_NOTICE("Python.h found, bindings will be built."),
      AC_MSG_NOTICE(
        "Python.h found, but swig was not.  Bindings will not be built.")),
    AC_MSG_NOTICE("Python.h not found.  Python bindings will not be built."))
], [
  AC_MSG_NOTICE("Swig not found.  No language bindings will be installed.")

  # Define the things that would have been defined.
  AM_CONDITIONAL([HAVE_PYTHON_DEV], test "x" != "x")
])



# Output files.
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
  Makefile
    src/Makefile
      src/paleo/Makefile
      src/dollarp/Makefile
      src/common/Makefile
    swig/Makefile
    tests/Makefile
      tests/paleo/Makefile
      tests/common/Makefile
    docs/Makefile])
AC_OUTPUT
