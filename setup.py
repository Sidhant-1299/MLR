from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
import pybind11
import os

eigen_path = os.path.abspath("external/eigen-3.4.0")
boost_path = os.path.abspath("externa;/boost1_88_0")

ext_modules = [
    Extension(
        'mlr_cpp',
        sources=['src/mlr_cpp.cpp', 'src/bindings.cpp'],
        include_dirs=[
            "include",
            eigen_path,
            boost_path,
            pybind11.get_include(),
            pybind11.get_include(user=True),
        ],
        language='c++',
        extra_compile_args=['-std=c++20']
    )
]



setup(
    name='mlr_cpp',
    version='1.0.0',
    ext_modules=ext_modules,
    packages=find_packages(),
    package_dir={'': '.'},
    include_package_data=True,
    description="A high-performance Multiple Linear Regression model implemented in C++ with a clean Python API via Pybind11 bindings",
    author="Sidhant Raj Khati",
    cmdclass={'build_ext': build_ext},
    zip_safe=False,
)
