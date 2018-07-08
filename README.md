# OpenDoorGL
[![Build Status](https://travis-ci.org/dmoody256/OpenDoorGL.svg?branch=master)](https://travis-ci.org/dmoody256/OpenDoorGL)
[![Build status](https://ci.appveyor.com/api/projects/status/iltlwjvcrb86v2e7/branch/master?svg=true)](https://ci.appveyor.com/project/dmoody256/opendoorgl/branch/master)
[![Coverage Status](https://coveralls.io/repos/github/dmoody256/OpenDoorGL/badge.svg?branch=master)](https://coveralls.io/github/dmoody256/OpenDoorGL?branch=master)

This is a simple OpenGL Rendering library I created for fun.

# Build

To build first install the dependencies:

```sudo apt-get install -y git cmake python-pip xorg-dev build-essential mesa-common-dev freeglut3-dev```

Then install SCons:

```
python -m pip install -U pip
pip install -U wheel setuptools
pip install SCons
```

Then run scons in the project directory:

```scons```

The build will output everything including example test applications into the build directory. To run one make sure to set the library path and run like this:

```
cd build/bin
export LD_LIBRARY_PATH=../lib
./RedCubeTest
```

# Testing

To run the graphics tests install SikuliX:

```sudo apt-get install -y sikuli-ide```

You can then run the test suite:

```scons --test```




