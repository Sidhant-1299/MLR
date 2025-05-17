#!/bin/zsh

export CC=/Users/macbookpro/bin/xpack-gcc-14.2.0-1/bin/gcc
export CXX=/Users/macbookpro/bin/xpack-gcc-14.2.0-1/bin/g++
source venv/vin
python3 -m build
pip3 install .
echo "REMOVING build\n"
rm MLR/mlr_cpp.cpython-312-darwin.so
echo "ADDING NEW BUILD\n"
cp build/lib.macosx-10.13-universal2-cpython-312/mlr_cpp.cpython-312-darwin.so MLR/
python3 MLR/test.py