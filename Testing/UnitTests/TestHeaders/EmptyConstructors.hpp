// MyTestSuite1.h
#include <cxxtest/TestSuite.h>

#include "odgl_WindowInterface.hpp"
#include "odgl_GLFW3.hpp"
#include "odgl_GeometricObject.hpp"

class GeomConstructorTest : public OpenDoorGL::GeometricObject
{
    public:
    void draw(OpenDoorGL::View *view)
    {
    }
};

class EmptyConstructorTest : public CxxTest::TestSuite
{
  public:
    void testGeometricObject(void)
    {
        OpenDoorGL::WindowInterface *mainWindow = new OpenDoorGL::GLFW3Window();
        if (mainWindow->CanCreateCoreProfile())
        {
            GeomConstructorTest geom;
        }
    }
};
