AM_CPPFLAGS = -I$(top_srcdir)/src/common
libcommon = $(top_builddir)/src/common/libcommon.la

noinst_LIBRARIES = libmockpoint.a
libmockpoint_a_SOURCES = \
	$(top_srcdir)/src/common/point.h \
	mock_point.c \
	mock_stroke.c

TESTS = check_stroke check_geom
check_PROGRAMS = check_stroke check_geom

check_stroke_SOURCES = stroke.c \
	$(top_srcdir)/src/common/point.h \
	$(top_srcdir)/src/common/stroke.h
check_stroke_CFLAGS = @CHECK_CFLAGS@
check_stroke_LDADD = $(libcommon) @CHECK_LIBS@

check_geom_SOURCES = geom.c \
	$(top_srcdir)/src/common/point.h \
	$(top_srcdir)/src/common/geom.h
check_geom_CFLAGS = @CHECK_CFLAGS@
check_geom_LDADD = $(libcommon) @CHECK_LIBS@
