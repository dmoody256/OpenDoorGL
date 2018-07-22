// MyTestSuite1.h
#include <cxxtest/TestSuite.h>

#include "odgl_WindowInterface.hpp"
#include "odgl_GLFW3.hpp"
#include "odgl_GeometricObject.hpp"
#include "odgl_Model.hpp"
#include "odgl_Cube.hpp"

class GeomConstructorTest : public OpenDoorGL::GeometricObject
{
  public:
    GeomConstructorTest() : OpenDoorGL::GeometricObject(false)
    {
    }
    void draw(OpenDoorGL::View *view)
    {
    }
};

class ModelConstructorTest : public OpenDoorGL::Model
{
  public:
    ModelConstructorTest() : OpenDoorGL::Model(false)
    {
    }
    void draw(OpenDoorGL::View *view)
    {
    }
};

class CubeConstructorTest : public OpenDoorGL::Cube
{
  public:
    CubeConstructorTest() : OpenDoorGL::Cube(false)
    {
    }
    void draw(OpenDoorGL::View *view)
    {
    }
};

class EmptyConstructorTest : public CxxTest::TestSuite
{
  public:
    void makeConstructors()
    {
        GeomConstructorTest *geom = new GeomConstructorTest();
        ModelConstructorTest *model = new ModelConstructorTest();
        CubeConstructorTest *cube = new CubeConstructorTest();

        delete geom;
        delete model;
        delete cube;
    }
    void testObjects(void)
    {
        TS_ASSERT_THROWS_NOTHING(makeConstructors());
    }
};
