AM_CPPFLAGS = -I$(top_srcdir)/src/common

TESTS = check_shapes
check_PROGRAMS = check_shapes
check_shapes_SOURCES = shapes.c \
	$(top_srcdir)/src/paleo/point.h \
	$(top_srcdir)/src/paleo/line.h \
	$(top_srcdir)/src/paleo/circle.h \
	$(top_srcdir)/src/paleo/curve.h \
	$(top_srcdir)/src/paleo/helix.h \
	$(top_srcdir)/src/paleo/spiral.h
check_shapes_CFLAGS = @CHECK_CFLAGS@
check_shapes_LDADD = \
	$(top_builddir)/src/common/libcommon.a \
	$(top_builddir)/src/paleo/libpaleo.a \
	@CHECK_LIBS@
