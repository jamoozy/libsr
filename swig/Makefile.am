SUBTASKS=
INSTALL_SUBTASKS=
UNINSTALL_SUBTASKS=
CLEAN_SUBTASKS=

if HAVE_PYTHON_DEV
SUBTASKS+=python
INSTALL_SUBTASKS+=install-python
UNINSTALL_SUBTASKS+=uninstall-python
CLEAN_SUBTASKS+=clean-python
endif


name = bindings
i_file = $(srcdir)/$(name).i
c_src_dir = $(srcdir)/../src
c_headers = $(c_src_dir)/paleo/paleo.h \
            $(c_src_dir)/common/stroke.h \
            $(c_src_dir)/common/point.h
libsr_a = $(builddir)/../src/.libs/libsr.a
libsr_so = $(builddir)/../src/.libs/libsr.so


all : $(SUBTASKS)

install-exec-local : $(INSTALL_SUBTASKS)

uninstall-local : $(UNINSTALL_SUBTASKS)

clean-local : $(CLEAN_SUBTASKS)


################################################################################
#                                    Python                                    #
################################################################################

py_wrap = $(srcdir)/$(name)_wrap.c
py_so = $(builddir)/_$(name).so
py_gen = $(builddir)/$(name).py
py_dst = $(libdir)/python2.7/libsr

python : $(py_so)

$(py_wrap) $(py_gen) : $(i_file) $(c_headers)
	swig -outdir $(builddir) -python -I$(c_src_dir) $(i_file)

$(py_so) : $(py_wrap) $(libsr_a)
	$(CC) -shared -fPIC -I$(c_src_dir) -I/usr/include/python2.7 $(CFLAGS) $(LDFLAGS) $^ -o $@

install-python :
	mkdir -p $(py_dst)
	cp $(py_gen) $(py_so) $(py_dst)
	cp $(srcdir)/libsr.py $(py_dst)/__init__.py

uninstall-python :
	rm -rf $(py_dst)

clean-python :
	rm -f $(py_so) $(py_wrap) $(py_gen)
