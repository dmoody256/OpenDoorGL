//#define ODGL_LOG_SCOPE
#define _USE_MATH_DEFINES

#include <GLEW/glew.h>

#include <glm/gtc/matrix_transform.hpp>

#include <sstream>
#include <vector>
#include <string>
#include <math.h>
#include <stdexcept>

#include "odgl_Logging.hpp"
#include "odgl_RenderObject.hpp"
#include "odgl_Group.hpp"

namespace OpenDoorGL
{

RenderObject::RenderObject()
    : _model(glm::mat4(1.0f)),
      _glInitialized(false),
      parent(nullptr)
{
}

RenderObject::~RenderObject()
{
}

void RenderObject::Update(double time_passed)
{
    ODGL_SCOPE();
}

bool RenderObject::unsetParent()
{
    if (this->parent != nullptr && !this->parent->containsObject(this))
    {
        ODGL_DEBUG_PRINT("ERROR: Tried to remove parent from a group that still contains the object.");
        return false;
    }
    return true;
}

bool RenderObject::setParent(Group *parent)
{

    if (parent == nullptr)
    {
        ODGL_DEBUG_PRINT("ERROR: tried to setParent to nullptr, use unsetParent to remove the parent.");
        return false;
    }

    if (parent->containsObject(this))
    {
        if (this->parent != nullptr)
        {
            if (this->parent->containsObject(this))
            {
                throw std::runtime_error("Two groups contained the same object!");
            }
            else
            {
                this->parent = parent;
                return true;
            }
        }
        else
        {
            this->parent = parent;
            return true;
        }
    }
    else
    {
        ODGL_DEBUG_PRINT("ERROR: tried to setParent to group which does not contain the object.");
        return false;
    }

} // namespace OpenDoorGL

Group *RenderObject::getParent()
{
    return this->parent;
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