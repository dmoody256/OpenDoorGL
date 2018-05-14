#ifndef ODGL_VIEW_H
#define ODGL_VIEW_H

#include "odgl_Include.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace OpenDoorGL
{

class ODGL_DLLEXPORT View
{
  public:
    View();
    ~View();

    glm::mat4 proj;
    glm::mat4 view;
    // Initial position : on +Z
    glm::vec3 position;
    // Initial horizontal angle : toward -Z
    float horizontalAngle;
    // Initial vertical angle : none
    float verticalAngle;
    // Initial Field of View
    float initialFoV;

    bool _orthographic;
    float speed;
    float mouseSpeed;
};

} // namespace OpenDoorGL

#endif