name = bindings

i_file = $(srcdir)/$(name).i
c_src_dir = $(srcdir)/../src
libsr_a = $(builddir)/../src/.libs/libsr.a
wrap_file = $(builddir)/$(name)_wrap.c
py_so = $(builddir)/_$(name).so
generated_py = $(builddir)/$(name).py

c_headers = $(c_src_dir)/paleo/paleo.h \
            $(c_src_dir)/common/stroke.h \
            $(c_src_dir)/common/point.h
lib_dst = $(libdir)/python2.7/libsr

all : $(py_so)

$(wrap_file) $(generated_py) : $(i_file) $(c_headers)
	swig -outdir $(builddir) -python -I$(c_src_dir) $(i_file)

$(py_so) : $(wrap_file) $(libsr_a)
	$(CC) -shared -fPIC -I$(c_src_dir) -I/usr/include/python2.7 $(CFLAGS) $(LDFLAGS) $^ -o $@

install-exec-local :
	mkdir -p $(lib_dst)
	cp $(generated_py) $(py_so) $(lib_dst)
	cp $(srcdir)/libsr.py $(lib_dst)/__init__.py

uninstall-local :
	rm -rf $(lib_dst)

clean-local :
	rm -f $(py_so) $(wrap_file) $(generated_py)
