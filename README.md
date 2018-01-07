# OpenDoorGL

This is a simple OpenGL Rendering library I created for fun.

# Build

To build first install the dependencies:

```sudo apt-get install git cmake python-pip xorg-dev```

Then isntall SCons:

```sudo pip install --egg SCons```

Then run scons in the project directory:

```scons```

The build will output everything including example test applications into the build directory. To run one make sure to set the library path and run like this:

```
cd build/bin
export LD_LIBRARY_PATH=../lib
./RedCubeTest
```

