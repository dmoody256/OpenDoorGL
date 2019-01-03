#define ODGL_LOG_SCOPE

#include <GLEW/glew.h>

#include <sstream>
#include <vector>
#include <string>

#include "odgl_Logging.hpp"
#include "odgl_Include.hpp"
#include "odgl_GeometricObject.hpp"
#include "odgl_ShaderManager.hpp"
#include "odgl_Vector.hpp"
namespace OpenDoorGL
{

GeometricObject::GeometricObject()
    : GeometricObject(true)
{
}

GeometricObject::GeometricObject(bool init_gl)
    : RenderObject(),
      _uniformV(0),
      _uniformM(0),
      _uniformSampler1(0),
      _uniformLight(0),
      _programID(0),
      _uniformMVP(0)
{
    {
        if (init_gl)
        {
            InitGL();
        }
    }
}

GeometricObject::~GeometricObject()
{
    ODGL_SCOPE();
    if (_glInitialized)
    {
        ODGL_INFO("Cleaning up OpenGL.");
        // Cleanup VBO and shader
        GL_CHECK(glDeleteBuffers(1, &_vertexBuffer));
        GL_CHECK(glDeleteBuffers(1, &_colorBuffer));
        GL_CHECK(glDeleteBuffers(1, &_textureBuffer));
        GL_CHECK(glDeleteBuffers(1, &_normalBuffer));
        GL_CHECK(glDeleteProgram(_programID));
    }
}

Vector GeometricObject::getMaxBounds()
{
    return Vector(0.0f, 0.0f, 0.0f);
}
Vector GeometricObject::getMinBounds()
{
    return Vector(0.0f, 0.0f, 0.0f);
}

float GeometricObject::getMinX()
{
    return getMinBounds().getX();
}
float GeometricObject::getMinY()
{
    return getMinBounds().getY();
}

float GeometricObject::getMinZ()
{
    return getMinBounds().getZ();
}
float GeometricObject::getMaxX()
{
    return getMaxBounds().getX();
}
float GeometricObject::getMaxY()
{
    return getMaxBounds().getY();
}
float GeometricObject::getMaxZ()
{
    return getMaxBounds().getZ();
}

void GeometricObject::InitGL()
{

    GL_CHECK(glGenBuffers(1, &_vertexBuffer));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer));

    GL_CHECK(glGenBuffers(1, &_colorBuffer));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer));

    GL_CHECK(glGenBuffers(1, &_textureBuffer));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _textureBuffer));

    GL_CHECK(glGenBuffers(1, &_normalBuffer));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer));

    _glInitialized = true;
}

void GeometricObject::setShadersFromString(const char *vert_shader, const char *frag_shader)
{
    if (_programID)
        GL_CHECK(glDeleteProgram(_programID));
    _programID = ShaderManager::LoadShadersFromString(vert_shader, vert_shader);
}

} // namespace OpenDoorGL