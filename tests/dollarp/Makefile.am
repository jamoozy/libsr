AM_CPPFLAGS = -I$(top_srcdir)/tests -I$(top_srcdir)/src/paleo

dollarp_shape_headers = $(top_srcdir)/src/dollarp/dollarp.h
libcommon = $(top_builddir)/src/common/.libs/libcommon.a
libdollarp = $(top_builddir)/src/dollarp/.libs/libdollarp.a
libs = $(libdollarp) $(libcommon)

TESTS = check_dollarp
check_PROGRAMS = check_dollarp

check_dollarp_SOURCES = dollarp.c
check_dollarp_CFLAGS = @CHECK_CFLAGS@
check_dollarp_LDADD = $(libs) @CHECK_LIBS@
