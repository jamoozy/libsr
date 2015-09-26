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
if HAVE_GO
SUBDIRS += go
endif



################################################################################
#                                Building Docs                                 #
################################################################################

# ---- Doxygen ----

html-local :
	$(srcdir)/docs/build.rb -v --indir="$(srcdir)" --outdir="$(builddir)"

# ---- README ----

readme : readme.html

readme.html : README.md
	marked -i $< -o $@
