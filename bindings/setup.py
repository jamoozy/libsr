import os
from distutils.core import setup, Extension

VERSION = [('MAJOR_VERSION', '0'),
           ('MINOR_VERSION', '1'),
           ('BUG_VERSION', '0')]

LONG_DESCRIPTION = """
A package that allows you to create a Stroke and write it to disk.
"""

# Use absolute paths for source files.  This allows us to run this script from
# any directory.
build_dir = os.path.join(os.getcwd(), '..')
dir_name = os.path.dirname(os.path.realpath(__file__))
base_dir = os.path.join(dir_name, '..')
src_dir = os.path.join(base_dir, 'src')

LIBSR_SO = os.path.join(build_dir, 'src', '.libs', 'libsr.so')
LIBSR_SO_DIR = os.path.dirname(LIBSR_SO)

libsr = Extension('libsr',
                  define_macros=VERSION,
                  include_dirs=['/usr/local/include',
                                os.path.join(src_dir, 'common')],
                  compile_args=['-x', 'C'],
                  extra_compile_args=['-std=gnu99', '-g',
                                      '-fPIC', '-DPIC',
                                      '-DHAVE_CONFIG_H', '-O2'],
                  extra_link_args=[LIBSR_SO],
                  library_dirs=[LIBSR_SO_DIR, '/usr/local/lib'],
                  runtime_library_dirs=[LIBSR_SO_DIR, '/usr/local/lib'],
                  sources=[os.path.join(dir_name, 'libsrmodule.c')],
                  depends=[LIBSR_SO])

setup(name='libsr',
      version='0.1',
      description='A sketch recognition package',
      long_description=LONG_DESCRIPTION,
      author='Andrew "Jamoozy" C. Sabisch',
      author_email='jamoozy@gmail.com',
      url='https://github.com/jamoozy/libsr',
      ext_modules=[libsr])
