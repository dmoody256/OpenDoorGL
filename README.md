# OpenDoorGL
[![Build Status](https://travis-ci.org/dmoody256/OpenDoorGL.svg?branch=master)](https://travis-ci.org/dmoody256/OpenDoorGL)
[![Build status](https://ci.appveyor.com/api/projects/status/iltlwjvcrb86v2e7/branch/master?svg=true)](https://ci.appveyor.com/project/dmoody256/opendoorgl/branch/master)
[![codecov](https://codecov.io/gh/dmoody256/OpenDoorGL/branch/master/graph/badge.svg)](https://codecov.io/gh/dmoody256/OpenDoorGL)

This is a simple OpenGL Rendering library I created for fun.

# Build

To build first install the dependencies:

```
sudo apt-get install -y git python-pip xorg-dev build-essential mesa-common-dev freeglut3-dev
```

Then install SCons:

```
python -m pip install -U pip
pip install -U wheel setuptools
pip install SCons
```

Then run scons in the project directory:

```
scons
```

The build will output everything including example test applications into the build directory. To run one make sure to set the library path and run like this:

```
cd build/bin
export LD_LIBRARY_PATH=../lib
./RedCubeTest
```

# Testing

You can run the unit tests by using the --unit option:

```
scons --unit
```

Unit test should be made to run in a headless environment or an environment without hardware accelerated graphics.

To run the graphics tests, which require hardware accelerated graphics, install SikuliX:

```
sudo apt-get install -y sikuli-ide
```

You can then run the test suite:

```
scons --test
```




