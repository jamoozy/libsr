ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src .
if HAVE_CHECK
SUBDIRS += tests
endif
if HAVE_ANY_LATEX
SUBDIRS += docs
endif
if HAVE_SWIG
SUBDIRS += swig
endif



################################################################################
#                           Building Docs with Doxygen                         #
################################################################################

html-local :
	doxygen $(srcdir)/Doxyfile
