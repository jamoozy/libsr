AM_CPPFLAGS = -I$(top_srcdir)/tests -I$(top_srcdir)/src/paleo

TESTS = check_shapes check_paleo check_recs
check_PROGRAMS = check_shapes check_paleo check_recs

check_shapes_SOURCES = shapes.c \
	$(top_srcdir)/src/common/point.h \
	$(top_srcdir)/src/paleo/line.h \
	$(top_srcdir)/src/paleo/circle.h \
	$(top_srcdir)/src/paleo/arc.h \
	$(top_srcdir)/src/paleo/curve.h \
	$(top_srcdir)/src/paleo/spiral.h \
	$(top_srcdir)/src/paleo/helix.h \
	$(top_srcdir)/src/paleo/complex.h
check_shapes_CFLAGS = @CHECK_CFLAGS@
check_shapes_LDADD = \
	$(top_builddir)/src/paleo/libpaleo.a \
	@CHECK_LIBS@

check_paleo_SOURCES = paleo.c \
	$(top_srcdir)/src/paleo/paleo.h
check_paleo_CFLAGS = @CHECK_CFLAGS@
check_paleo_LDADD = \
	$(top_builddir)/tests/common/libmockpoint.a \
	$(top_builddir)/src/paleo/libpaleo.a \
	@CHECK_LIBS@

check_recs_SOURCES = recs.c \
	$(top_srcdir)/src/common/point.h \
	$(top_srcdir)/src/paleo/line.h \
	$(top_srcdir)/src/paleo/circle.h \
	$(top_srcdir)/src/paleo/arc.h \
	$(top_srcdir)/src/paleo/curve.h \
	$(top_srcdir)/src/paleo/spiral.h \
	$(top_srcdir)/src/paleo/helix.h \
	$(top_srcdir)/src/paleo/complex.h
check_recs_CFLAGS = @CHECK_CFLAGS@
check_recs_LDADD = \
	$(top_builddir)/src/paleo/libpaleo.a \
	@CHECK_LIBS@
