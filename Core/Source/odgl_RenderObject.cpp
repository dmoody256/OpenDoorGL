//#define ODGL_LOG_SCOPE

#include <GLEW/glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include <sstream>
#include <vector>
#include <string>

#include "odgl_Logging.hpp"
#include "odgl_RenderObject.hpp"

namespace OpenDoorGL
{

RenderObject::RenderObject() : _model(glm::mat4(1.0f)),
                               _glInitialized(false)
{
}

RenderObject::~RenderObject()
{
}

void RenderObject::Update(double time_passed)
{
    ODGL_SCOPE();
}

void RenderObject::Translate(float x, float y, float z)
{
    glm::vec3 trans(x, y, z);
    _model = glm::translate(_model, trans);
}
} // namespace OpenDoorGL