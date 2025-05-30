from setuptools import setup, Extension, find_packages
from setuptools.command.build_ext import build_ext
import pybind11
import os
import sys

# Try to detect if we're building in like GitHub Actions
IS_CI = (
    os.environ.get("CI", "false").lower() == "true" or
    os.environ.get("CIBUILDWHEEL", "0").strip() == "1"
)

is_windows = sys.platform == "win32"

compiler_warning_flag = '/W0' if is_windows else "-w" #suppress compiler warnings

# eigen_path = os.path.abspath("external/eigen-3.4.0")
# boost_path = os.path.abspath("external/boost_1_88_0")

# if IS_CI :
#     if sys.platform == "darwin":
#         boost_path = "/usr/local/include/boost" #symlink path
#         eigen_path = "/usr/local/include/Eigen"
#     elif sys.platform == 'linux': #not reachable cause my machine is mac
#         eigen_path = "/usr/include/Eigen"
#         boost_path = "/usr/include/"

# Replace your path detection with:
eigen_path = (
    os.environ.get("EIGEN_INCLUDE_PATH") or 
    os.path.abspath("external/eigen-3.4.0")
)
boost_path = (
    os.environ.get("BOOST_INCLUDE_PATH") or 
    os.path.abspath("external/boost_1_88_0")
)


class BuildExtensions(build_ext):
    def build_extensions(self):
        # log_build_system(self)
        for ext in self.extensions:
            print(f"self.extensions: {self.extensions}")
            ext.include_dirs.append(eigen_path)
            ext.include_dirs.append(boost_path)
            ext.include_dirs.append(pybind11.get_include())
            ext.include_dirs.append(pybind11.get_include(user=True))

        print("Env variables: ")
        for k, v in os.environ.items():
            print(f"{k} = {v}")
        print("Logging build info: ")
        print("==== BuildExtWithCheck: Starting extension build ====\n")
        print(f"Detected platform: {sys.platform}\n")
        print(f"CI Environment: {IS_CI}\n")
        print(f"Using Eigen path: {eigen_path}\n")
        print(f"Using Boost path: {boost_path}\n")
        print(f"Using Pybind11 includes: {pybind11.get_include()}, {pybind11.get_include(user=True)}\n")
        print("\n=== Extensions ===\n")
        super().build_extensions()

#defines how to compile the C++ code
ext_modules = [
    Extension(
        'mlr_cpp',
        sources=['src/mlr_cpp.cpp', 'src/bindings.cpp'],
        include_dirs=[
            "include",  
        ],
        language='c++',
        extra_compile_args=['-std=c++20',compiler_warning_flag]
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
    cmdclass={'build_ext': BuildExtensions}, #middleware for customizing the build (compilation)
    zip_safe=False, #Basically unsafe to zip; don't zip me; unzip me; zipped .so/.dll files might not load properly
)



