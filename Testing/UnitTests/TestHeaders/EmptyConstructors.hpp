// MyTestSuite1.h
#include <cxxtest/TestSuite.h>

#include "odgl_Model.hpp"

class EmptyConstructorTest : public CxxTest::TestSuite
{
  public:
    void testGeometricObject(void)
    {
        WindowInterface *mainWindow = new GLFW3Window();
        if (mainWindow->CanCreateCoreProfile())
        {
            GeometricObject geom;
            TS_ASSERT_DIFFERS(std::find(vecVerts.begin(), vecVerts.end(), glm::vec3(-0.242188, 0.242188, 0.796875)), vecVerts.end());
        }
    }
};
