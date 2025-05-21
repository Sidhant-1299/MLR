from setuptools import find_packages, setup, Extension
import pybind11
from setuptools.command.build_ext import build_ext
import os


here = os.path.abspath(os.path.dirname(__file__))

ext_modules = [
    Extension(
        'mlr_cpp',
        sources=['src/mlr_cpp.cpp', 'src/bindings.cpp'],
        include_dirs=[
            pybind11.get_include(),                   # Pybind11 headers
            pybind11.get_include(user=True),         # For user install cases
            "/Users/macbookpro/Desktop/MachineLearning/MLR/include/",
            "/Users/macbookpro/Desktop/MachineLearning/MLR/external/eigen-3.4.0",
            "/Users/macbookpro/Desktop/MachineLearning/MLR/external/boost_1_88_0",                       
        ],
        language='c++',
        extra_compile_args=['-std=c++23']
    )
]


setup(
    name='MLR',
    version='1.0.0',
    ext_modules=ext_modules,
    packages=find_packages(),
    package_dir={'': '.'},
    description="A high-performance Multiple Linear Regression model implemented in C++ with a clean Python API via Pybind11 bindings",
    author="Sidhant Raj Khati",
    cmdclass={'build_ext': build_ext},
    zip_safe=False,
)

