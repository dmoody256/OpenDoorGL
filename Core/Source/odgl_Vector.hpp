#ifndef ODGL_VECTOR_H
#define ODGL_VECTOR_H

#include "odgl_Include.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace OpenDoorGL
{

class ODGL_DLLEXPORT Vector
{
public:
    Vector();
    Vector(float x, float y, float z);
    ~Vector();

    float getX();
    float getY();
    float getZ();

    void setX(float value);
    void setY(float value);
    void setZ(float value);

protected:
    float x;
    float y;
    float z;

};
}

#endif