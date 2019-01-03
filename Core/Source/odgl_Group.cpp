#define _USE_MATH_DEFINES

#include <GLEW/glew.h>

#include <vector>
#include <string>
#include <iostream>
#include <math.h>

#include <glm/gtc/matrix_transform.hpp>

#include "odgl_Group.hpp"
#include "odgl_GeometricObject.hpp"
#include "odgl_Cube.hpp"
#include "odgl_Model.hpp"
#include "odgl_Button.hpp"
#include "odgl_Vector.hpp"

namespace
{
float getMidPoint(float min, float max)
{
    return ((max - min) / 2.0f) + min;
}
} // namespace

namespace OpenDoorGL
{

Group::Group()
    : minBounds(0.0f, 0.0f, 0.0f),
      maxBounds(0.0f, 0.0f, 0.0f)
{
}

Group::~Group()
{
    for (int i = 0; i < objects.size(); i++)
    {
        switch (objects.at(i).first)
        {
        case ObjectType::RenderObjectType:
        case ObjectType::UnknownType:
            delete objects.at(i).second;
            break;
        case ObjectType::GeometricObjectType:
            delete static_cast<GeometricObject *>(objects.at(i).second);
            break;
        case ObjectType::CubeType:
            delete static_cast<Cube *>(objects.at(i).second);
            break;
        case ObjectType::ModelType:
            delete static_cast<Model *>(objects.at(i).second);
            break;
        case ObjectType::ButtonType:
            delete static_cast<Button *>(objects.at(i).second);
            break;
        default:
            delete objects.at(i).second;
        }
    }
}

void Group::draw(View *view)
{
    for (int i = 0; i < objects.size(); i++)
    {
        objects.at(i).second->draw(view);
    }
}

void Group::Rotate(float degrees, float x, float y, float z)
{

    glm::vec3 trans(x, y, z);
    glm::vec3 center(getCenterPoint().getX(), getCenterPoint().getY(), getCenterPoint().getZ());
    _model = glm::translate(_model, center);
    _model = glm::rotate(_model, (float)(degrees / 90.0f * M_PI), trans);
    _model = glm::translate(_model, -center);
}

bool Group::removeObject(RenderObject *object)
{
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects.at(i).second == object)
        {
            // found the object, remove it and unset the parent
            objects.erase(objects.begin() + i);
            return object->unsetParent();
        }
    }
    // object was not in the group to begin with
    return true;
}

bool Group::containsObject(RenderObject *object)
{
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects.at(i).second == object)
        {
            return true;
        }
    }
    return false;
}

Vector Group::getCenterPoint()
{
    return Vector(
        getMidPoint(minBounds.getX(), maxBounds.getX()),
        getMidPoint(minBounds.getY(), maxBounds.getY()),
        getMidPoint(minBounds.getZ(), maxBounds.getZ()));
}

void Group::updateBoundingBox()
{
    for (int i = 0; i < objects.size(); i++)
    {
        GeometricObject *obj = dynamic_cast<GeometricObject *>(objects.at(i).second);
        if (obj != nullptr)
        {
            Vector bounds = obj->getMinBounds();
            if (bounds.getX() < minBounds.getX())
            {
                minBounds.setX(bounds.getX());
            }
            if (bounds.getY() < minBounds.getY())
            {
                minBounds.setY(bounds.getY());
            }
            if (bounds.getZ() < minBounds.getZ())
            {
                minBounds.setZ(bounds.getZ());
            }

            bounds = obj->getMaxBounds();
            if (bounds.getX() > maxBounds.getX())
            {
                maxBounds.setX(bounds.getX());
            }
            if (bounds.getY() > maxBounds.getY())
            {
                maxBounds.setY(bounds.getY());
            }
            if (bounds.getZ() > maxBounds.getZ())
            {
                maxBounds.setZ(bounds.getZ());
            }
        }
    }
}

bool Group::InsertObject(RenderObject *object, bool checkType)
{
    if (object == nullptr)
    {
        return false;
    }

    if (checkType)
    {
        if (dynamic_cast<GeometricObject *>(object))
        {
            if (dynamic_cast<Cube *>(object))
            {
                return InsertObject(object, ObjectType::CubeType);
            }

            if (dynamic_cast<Model *>(object))
            {
                return InsertObject(object, ObjectType::ModelType);
            }

            if (dynamic_cast<Button *>(object))
            {
                return InsertObject(object, ObjectType::ButtonType);
            }

            return InsertObject(object, ObjectType::GeometricObjectType);
        }
        else
        {
            if (dynamic_cast<Group *>(object))
            {
                return InsertObject(object, ObjectType::GroupType);
            }
        }
    }

    return InsertObject(object, ObjectType::RenderObjectType);
}

bool Group::InsertObject(RenderObject *object, enum ObjectType objectType)
{
    objects.push_back(std::make_pair(objectType, object));
    if (object->setParent(this))
    {
        updateBoundingBox();
        return true;
    }
    return false;
} // namespace OpenDoorGL

bool Group::InsertObject(Cube *object)
{
    return InsertObject(object, ObjectType::CubeType);
}

bool Group::InsertObject(Model *object)
{
    return InsertObject(object, ObjectType::ModelType);
}

bool Group::InsertObject(Button *object)
{
    return InsertObject(object, ObjectType::ButtonType);
}

bool Group::InsertObject(Group *object)
{
    return InsertObject(object, ObjectType::GroupType);
}

void Group::Update(double time_passed)
{
    for (int i = 0; i < objects.size(); i++)
    {
        objects.at(i).second->Update(time_passed);
    }
}

} // namespace OpenDoorGL
