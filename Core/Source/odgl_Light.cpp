#include "odgl_Light.hpp"

namespace OpenDoorGL
{

Light::Light()
    : position(glm::vec3(0, 0, 5))
{
}

Light::~Light()
{
}

} // namespace OpenDoorGL