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
    void draw(OpenDoorGL::View *view)
    {
    }
};

class ModelConstructorTest : public OpenDoorGL::Model
{
  public:
    void draw(OpenDoorGL::View *view)
    {
    }
};

class CubeConstructorTest : public OpenDoorGL::Cube
{
  public:
    void draw(OpenDoorGL::View *view)
    {
    }
};

class EmptyConstructorTest : public CxxTest::TestSuite
{
  public:
    void testObjects(void)
    {
        OpenDoorGL::WindowInterface *mainWindow = new OpenDoorGL::GLFW3Window();
        if (mainWindow->CanCreateCoreProfile())
        {
            GeomConstructorTest *geom = new GeomConstructorTest();
            ModelConstructorTest *model = new ModelConstructorTest();
            CubeConstructorTest *cube = new CubeConstructorTest();

            delete geom;
            delete model;
            delete cube;
        }
        else
        {
            TS_SKIP("Can't create OpenGL Core Profile.");
        }
    }
};
