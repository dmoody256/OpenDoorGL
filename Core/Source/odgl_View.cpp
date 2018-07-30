#include "odgl_View.hpp"

namespace OpenDoorGL
{

View::View()
    : position(glm::vec3(0, 0, 5)),
      horizontalAngle(3.14f),
      verticalAngle(0.0f),
      initialFoV(45.0f),
      speed(3.0f),
      mouseSpeed(0.005f),
      _orthographic(false)
{
}

View::~View()
{
}

} // namespace OpenDoorGL