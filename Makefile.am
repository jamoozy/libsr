SUBDIRS = src bindings .
if HAVE_CHECK
SUBDIRS += tests
endif
if HAVE_ANY_LATEX
SUBDIRS += docs
endif

pymodule-sdist:
	$(MAKE) -C bindings pymodule-sdist
