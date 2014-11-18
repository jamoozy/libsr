AM_CPPFLAGS = -I$(top_srcdir)/src/common

TESTS = check_stroke
check_PROGRAMS = check_stroke
check_stroke_SOURCES = stroke.c \
	$(top_srcdir)/src/common/point.h \
	$(top_srcdir)/src/common/stroke.h
check_stroke_CFLAGS = @CHECK_CFLAGS@
check_stroke_LDADD = $(top_builddir)/src/common/libcommon.a @CHECK_LIBS@
