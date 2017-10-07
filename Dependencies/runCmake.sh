#! /bin/sh

cd glfw
mkdir -p build
cd build
cmake -DCMAKE_INSTALL_PREFIX=../../ -DBUILD_SHARED_LIBS=On ..
make -j8
make install
