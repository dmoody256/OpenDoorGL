// MyTestSuite1.h
#include <cxxtest/TestSuite.h>

#include "odgl_RenderObject.hpp"
#include <glm/glm.hpp>

class TestRenderObject : public OpenDoorGL::RenderObject{
public:
    glm::mat4 getModelMatrix(){
        return _model;
    }

    void draw(OpenDoorGL::View* view){

    }
};

class TranslateTest : public CxxTest::TestSuite
{
public:

    void testTranslate(void)
    {
        TestRenderObject obj;
        TS_ASSERT_EQUALS(obj.getModelMatrix(),  glm::mat4(1.0f));
        obj.Translate(10, 0, 0);
        TS_ASSERT_EQUALS(obj.getModelMatrix(), glm::translate( glm::mat4(1.0f), glm::vec3(10,0,0)));
    }
};

