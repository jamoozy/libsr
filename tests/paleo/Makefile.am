AM_CPPFLAGS = -I$(top_srcdir)/tests -I$(top_srcdir)/src/paleo

TESTS = check_shapes
check_PROGRAMS = check_shapes
check_shapes_SOURCES = shapes.c \
	$(top_srcdir)/src/common/point.h \
	$(top_srcdir)/src/paleo/line.h \
	$(top_srcdir)/src/paleo/circle.h \
	$(top_srcdir)/src/paleo/curve.h \
	$(top_srcdir)/src/paleo/helix.h \
	$(top_srcdir)/src/paleo/spiral.h
check_shapes_CFLAGS = @CHECK_CFLAGS@
check_shapes_LDADD = \
	$(top_builddir)/src/paleo/libpaleo.a \
	$(top_builddir)/tests/common/libmockpoint.a \
	@CHECK_LIBS@
