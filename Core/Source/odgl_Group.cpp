
#include <GLEW/glew.h>

#include <vector>
#include <string>

#include "odgl_Group.hpp"
#include "odgl_GeometricObject.hpp"
#include "odgl_Cube.hpp"
#include "odgl_Model.hpp"
#include "odgl_Button.hpp"
namespace OpenDoorGL
{

Group::Group()
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
    for (int i = 0; i < objects.size(); i++)
    {
        objects.at(i).second->Rotate(degrees, x, y, z);
    }
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
    return object->setParent(this);
}

bool Group::InsertObject(GeometricObject *object)
{
    return InsertObject(object, ObjectType::GeometricObjectType);
}

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
