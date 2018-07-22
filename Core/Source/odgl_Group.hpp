#ifndef ODGL_GROUP_H
#define ODGL_GROUP_H

#include <GLEW/glew.h>

#include <vector>
#include <string>

#include "odgl_Include.hpp"
#include "odgl_RenderObject.hpp"
#include "odgl_GeometricObject.hpp"
#include "odgl_Cube.hpp"
#include "odgl_Model.hpp"
#include "odgl_Button.hpp"

namespace OpenDoorGL
{

class ODGL_DLLEXPORT Group : public RenderObject
{

public:
  Group();
  ~Group();
  virtual void draw(View *view);
  virtual void InsertObject(RenderObject *object, bool checkType = true);
  virtual void InsertObject(GeometricObject *object);
  virtual void InsertObject(Cube *object);
  virtual void InsertObject(Button *object);
  virtual void InsertObject(Model *object);
  virtual void Update(double time_passed);

protected:
  enum ObjectType
  {
    RenderObjectType,
    GeometricObjectType,
    CubeType,
    ModelType,
    ButtonType,
    UnknownType
  };

  std::vector<std::pair<ObjectType, RenderObject *>> objects;
};
} // namespace OpenDoorGL

#endif