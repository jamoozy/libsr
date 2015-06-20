i_file = $(srcdir)/bindings.i
c_src_dir = $(srcdir)/../src
libcommon_a = $(builddir)/../src/common/.libs/libcommon.a
wrap_file = $(srcdir)/bindings_wrap.c
py_so = $(builddir)/_bindings.so

c_headers = $(c_src_dir)/common/stroke.h \
            $(c_src_dir)/common/point.h
lib_dst = $(libdir)/python2.7/libsr

all : $(py_so)

$(wrap_file) : $(i_file) $(c_headers)
	swig -python -outdir $(builddir) -I$(c_src_dir) $(i_file)

$(py_so) : $(wrap_file) $(libcommon_a)
	gcc -shared -fPIC -I$(c_src_dir) -I/usr/include/python2.7 $(CFLAGS) $(LDFLAGS) $^ -o $@

install-exec-local :
	mkdir -p $(lib_dst)
	cp $(builddir)/bindings.py $(py_so) $(lib_dst)
	cp $(srcdir)/libsr.py $(lib_dst)/__init__.py
	pushd $(lib_dst)
	python -c 'import .'
	popd

clean-local :
	rm -f $(py_so) $(wrap_file)
