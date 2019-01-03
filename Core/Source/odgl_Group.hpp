#ifndef ODGL_GROUP_H
#define ODGL_GROUP_H

#include <GLEW/glew.h>

#include <vector>
#include <string>

#include "odgl_Include.hpp"
#include "odgl_RenderObject.hpp"
#include "odgl_Vector.hpp"

namespace OpenDoorGL
{

class Cube;
class Button;
class Model;
class Group;
class GeometricObject;
class Vector;

class ODGL_DLLEXPORT Group : public RenderObject
{

public:
  Group();
  virtual ~Group();

  void draw(View *view) override;
  bool InsertObject(RenderObject *object, bool checkType = true);

  // specific inserts
  bool InsertObject(GeometricObject *object);
  bool InsertObject(Cube *object);
  bool InsertObject(Button *object);
  bool InsertObject(Model *object);
  bool InsertObject(Group *object);

  bool removeObject(RenderObject *object);
  void updateBoundingBox();

  Vector getCenterPoint() override;

  void Update(double time_passed) override;
  void Rotate(float degrees, float x, float y, float z) override;

  // Checks if the passed object is contained in this group.
  bool containsObject(RenderObject *object);

protected:
  enum ObjectType
  {
    RenderObjectType,
    GeometricObjectType,
    CubeType,
    ModelType,
    ButtonType,
    GroupType,
    UnknownType
  };

  std::vector<std::pair<ObjectType, RenderObject *>> objects;

  Vector minBounds;
  Vector maxBounds;

private:
  bool InsertObject(RenderObject *object, enum ObjectType objectType);
};
} // namespace OpenDoorGL

#endif