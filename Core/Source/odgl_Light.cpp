#include "odgl_Light.hpp"

namespace OpenDoorGL
{

Light::Light()
    : position(glm::vec3(0.0f, 0.0f, 0.0f)),
      color(glm::vec3(1.0f, 1.0f, 1.0f)),
      power(20.0f)

{
}

Light::~Light()
{
}

} // namespace OpenDoorGL