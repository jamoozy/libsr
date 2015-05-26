import os
from distutils.core import setup, Extension


# Use absolute paths for source files.  This allows us to run this script from
# any directory.
build_dir = os.path.join(os.getcwd(), '..')
dir_name = os.path.dirname(os.path.realpath(__file__))
base_dir = os.path.join(dir_name, '..')
src_dir = os.path.join(base_dir, 'src')

libsr = Extension('libsr',
                  define_macros = [('MAJOR_VERSION', '0'),
                                   ('MINOR_VERSION', '1')],
                  include_dirs = ['/usr/local/include',
                                  os.path.join(src_dir, 'common')],
                  extra_link_args = [os.path.join(build_dir,
                      'src', 'common', 'libcommon.a')],
                  library_dirs = ['/usr/local/lib'],
                  sources = [os.path.join(dir_name, 'libsrmodule.c')])

setup (name = 'libsr',
       version = '0.1',
       description = 'A sketch recognition package',
       author = 'Andrew "Jamoozy" C. Sabisch',
       author_email = 'jamoozy@gmail.com',
       url = 'https://github.com/jamoozy/libsr',
       long_description = '''
A package that allows you to create a Stroke and write it to disk.
''',
       ext_modules = [libsr])
