#ifndef ODGL_GROUP_H
#define ODGL_GROUP_H

#include <GLEW/glew.h>

#include <vector>
#include <string>

#include "odgl_Include.hpp"
#include "odgl_RenderObject.hpp"

namespace OpenDoorGL
{

class ODGL_DLLEXPORT Group : public RenderObject
{

  public:
    Group();
    ~Group();
    virtual void draw(View *view);
    virtual void InsertObject(RenderObject *object);
    virtual void Update(double time_passed);

  protected:
    std::vector<RenderObject *> objects;
};
} // namespace OpenDoorGL

#endif