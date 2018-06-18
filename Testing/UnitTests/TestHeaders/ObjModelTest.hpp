// MyTestSuite1.h
#include <cxxtest/TestSuite.h>

#include "odgl_Model.hpp"
#include "odgl_ModelLoader.hpp"
#include <glm/glm.hpp>

#include <algorithm>

class TestModelObject : public OpenDoorGL::Model
{
  public:
    void draw(OpenDoorGL::View *view)
    {
    }

    std::vector<GLfloat> getVertices()
    {
        return _vertices;
    }
};

class ObjModelTest : public CxxTest::TestSuite
{
  public:
    void testLoadObj(void)
    {
        TestModelObject *testObject = static_cast<TestModelObject *>(OpenDoorGL::ModelLoader::LoadOBJFile("resources/LoadObjTest.obj"));
        std::vector<GLfloat> verts = testObject->getVertices();
        std::vector<glm::vec3> vecVerts;
        for (int i = 0; i < verts.size(); i += 3)
        {
            glm::vec3 vertVec = glm::vec3(verts.at(i), verts.at(i + 1), verts.at(i + 2));
            vecVerts.push_back(vertVec);
        }
        TS_ASSERT_DIFFERS(std::find(vecVerts.begin(), vecVerts.end(), glm::vec3(-0.242188, 0.242188, 0.796875)), vecVerts.end());
    }
};
