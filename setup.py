from distutils.core import setup, Extension

demo = Extension(
    'demo',
    sources=['demo.c'])

description = (
    'Demonstrate some problems running a C extension module in mod_wsgi.')

setup(
    name='Demo',
    version='0.0',
    description=description,
    py_modules=['mymodule'],
    ext_modules=[demo])
