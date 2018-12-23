//#define ODGL_LOG_SCOPE
#define _USE_MATH_DEFINES

#include <GLEW/glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include <sstream>
#include <vector>
#include <string>
#include <math.h>

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

void RenderObject::Rotate(float degrees, float x, float y, float z)
{
    glm::vec3 trans(x, y, z);
    _model = glm::rotate(_model, (float)(degrees / 90.0f * M_PI), trans);
}

void RenderObject::Rotate(float degrees, float x, float y, float z, float pivot_x, float pivot_y, float pivot_z)
{
    glm::vec3 trans(x, y, z);
    glm::vec3 pivot_vec(pivot_x, pivot_y, pivot_z);
    glm::vec3 pivot_back_vec(-pivot_x, -pivot_y, -pivot_z);
    _model = glm::translate(_model, pivot_vec);
    _model = glm::rotate(_model, (float)(degrees / 90.0f * M_PI), trans);
    _model = glm::translate(_model, pivot_back_vec);
}

} // namespace OpenDoorGL