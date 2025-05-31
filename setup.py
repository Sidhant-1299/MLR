from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
import pybind11
import os
import sys

# Try to detect if we're building in like GitHub Actions
IS_CI = (
    (os.environ.get("CI", "false").strip().lower() == "true") or 
    (os.environ.get("CIBUILDWHEEL","0").strip() == "1")
    )

is_windows = sys.platform == "win32"

warning_flag = '/W0' if is_windows else '-w'

# Use external/ Eigen and Boost by default for sdist
eigen_path = os.path.abspath("external/eigen-3.4.0")
boost_path = os.path.abspath("external/boost_1_88_0")

#point to system wide eigen and boost for CI
# eigen_path = "/usr/include/eigen3" if (IS_CI and not is_windows) else default_eigen_path
# boost_path = "/usr/include/boost" if (IS_CI and not is_windows) else default_boost_path

if IS_CI and not is_windows:
    eigen_path =  "/usr/include/Eigen"
    boost_path =  "/usr/include/boost"


class BuildExtWithCheck(build_ext):
    def build_extensions(self):
        for ext in self.extensions:
            ext.include_dirs.append(eigen_path)
            ext.include_dirs.append(boost_path)
            ext.include_dirs.append(pybind11.get_include())
            ext.include_dirs.append(pybind11.get_include(user=True))
        super().build_extensions()

ext_modules = [
    Extension(
        'mlr_cpp',
        sources=['src/mlr_cpp.cpp', 'src/bindings.cpp'],
        include_dirs=[
            "include",  
        ],
        language='c++',
        extra_compile_args=['-std=c++20',warning_flag]
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
    cmdclass={'build_ext': BuildExtWithCheck},
    zip_safe=False,
)
