#ifndef ODGL_GROUP_H
#define ODGL_GROUP_H

#include <GLEW/glew.h>

#include <vector>
#include <string>

#include "odgl_Include.hpp"
#include "odgl_RenderObject.hpp"
#include "odgl_GeometricObject.hpp"

namespace OpenDoorGL
{

class ODGL_DLLEXPORT Group : public RenderObject
{
  
  public:
    Group();
    ~Group();
    virtual void draw(View *view);
    virtual void InsertObject(RenderObject *object);
    virtual void InsertObject(GeometricObject *object);
    virtual void Update(double time_passed);

  protected:

    enum ObjectType{
      RenderObjectType,
      GeometricObjectType,
      UnknownType
    };

    std::vector<std::pair<ObjectType, RenderObject*> > objects;
};
} // namespace OpenDoorGL

#endif