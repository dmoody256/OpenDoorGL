// MyTestSuite1.h
#include <cxxtest/TestSuite.h>

#include "odgl_GLFW3.hpp"
#include "odgl_GeometricObject.hpp"

class EmptyConstructorTest : public CxxTest::TestSuite
{
  public:
    void testGeometricObject(void)
    {
        WindowInterface *mainWindow = new GLFW3Window();
        if (mainWindow->CanCreateCoreProfile())
        {
            GeometricObject geom;
        }
    }
};
