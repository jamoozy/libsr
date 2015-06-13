ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src swig .
if HAVE_CHECK
SUBDIRS += tests
endif
if HAVE_ANY_LATEX
SUBDIRS += docs
endif

pymodule-sdist:
	$(MAKE) -C bindings pymodule-sdist
