#ifndef ODGL_RENDER_OBJECT_H
#define ODGL_RENDER_OBJECT_H

#include "odgl_Include.hpp"

#include <glm/glm.hpp>

namespace OpenDoorGL
{

class View;
class Group;

class ODGL_DLLEXPORT RenderObject
{

public:
  RenderObject();
  ~RenderObject();

  void Translate(float x, float y, float z);
  virtual void Rotate(float degrees, float x, float y, float z);
  virtual void Rotate(float degrees, float x, float y, float z, float pivot_x, float pivot_y, float pivot_z);

  virtual void draw(View *view) = 0;
  virtual void Update(double time_passed);
  bool setParent(Group *parent);
  bool unsetParent();
  Group *getParent();

protected:
  glm::mat4 _model;
  bool _glInitialized;
  Group *parent;
};
} // namespace OpenDoorGL

#endif