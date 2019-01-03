#include "odgl_Vector.hpp"

#include <glm/glm.hpp>

namespace OpenDoorGL
{

Vector::Vector()
:   Vector(0.0f, 0.0f, 0.0f)
{
}

Vector::Vector(float x, float y, float z)
:   x(x),
    y(y),
    z(z)
{
}

Vector::~Vector()
{

}

float Vector::getX()
{
    return x;
}

float Vector::getY()
{
    return y;
}

float Vector::getZ()
{
    return z;
}

void Vector::setX(float value) 
{
    x = value;
}

void Vector::setY(float value)
{
    y = value;
}

void Vector::setZ(float value)
{
    z = value;
}

}


