#ifndef ODGL_LIGHT_H
#define ODGL_LIGHT_H

#include "odgl_Include.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace OpenDoorGL
{

class ODGL_DLLEXPORT Light
{
public:
  Light();
  ~Light();

  // Initial position : on +Z
  glm::vec3 position;
};

} // namespace OpenDoorGL

#endif