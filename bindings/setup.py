from distutils.core import setup, Extension

libsr = Extension('libsr',
                  define_macros = [('MAJOR_VERSION', '0'),
                                   ('MINOR_VERSION', '1')],
                  include_dirs = ['/usr/local/include'],
                  libraries = ['../src/libsr.a'],
                  library_dirs = ['/usr/local/lib'],
                  sources = ['libsrmodule.c'])

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
