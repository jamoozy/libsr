name = bindings

i_file = $(srcdir)/$(name).i
c_src_dir = $(srcdir)/../src
wrap_file = $(srcdir)/$(name)_wrap.c
py_so = $(builddir)/_$(name).so
generated_py = $(builddir)/$(name).py

c_headers = $(c_src_dir)/paleo/paleo.h \
            $(c_src_dir)/common/stroke.h \
            $(c_src_dir)/common/point.h
lib_dst = $(libdir)/python2.7/libsr



################################################################################
#                               General Targets                                #
################################################################################

all : build-python

install-exec-local : install-exec-local-python

uninstall-local : uninstall-local-python

clean-local : clean-local-python



################################################################################
#                           Python-specific Targets                            #
################################################################################

# ---- build ----

build-python :$(py_so)

$(wrap_file) $(generated_py) : $(i_file) $(c_headers)
	swig -outdir $(builddir) -python -I$(c_src_dir) $(i_file)

$(py_so) : $(wrap_file)
	$(CC) -shared -fPIC -R/usr/local/lib -R/usr/lib -I$(c_src_dir) -I/usr/include/python2.7 $(CFLAGS) $(LDFLAGS) $^ -o $@


# ---- (un)install ----

install-exec-local-python :
	mkdir -p $(lib_dst)
	cp $(generated_py) $(py_so) $(lib_dst)
	cp $(srcdir)/libsr.py $(lib_dst)/__init__.py

uninstall-local-python :
	rm -rf $(lib_dst)


# ---- clean ----

clean-local-python :
	rm -f $(py_so) $(wrap_file) $(generated_py)
