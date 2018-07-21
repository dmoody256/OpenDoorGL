
#include <GLEW/glew.h>

#include <vector>
#include <string>

#include "odgl_Group.hpp"

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
            delete (GeometricObject *)objects.at(i).second;
            break;
        case ObjectType::CubeType:
            delete (Cube *)objects.at(i).second;
            break;
        default:
            delete objects.at(i).second;
            break;
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

void Group::InsertObject(RenderObject *object, bool checkType)
{
    if (checkType)
    {
        GeometricObject *geomObject = dynamic_cast<GeometricObject *>(object);
        if (geomObject)
        {

            Cube *cubeObject = dynamic_cast<Cube *>(object);
            if (cubeObject)
            {
                objects.push_back(std::make_pair(ObjectType::CubeType, cubeObject));
                return;
            }

            objects.push_back(std::make_pair(ObjectType::CubeType, cubeObject));
            return;
        }
    }
    else
    {
        objects.push_back(std::make_pair(ObjectType::RenderObjectType, object));
    }
}

void Group::InsertObject(GeometricObject *object)
{
    objects.push_back(std::make_pair(ObjectType::GeometricObjectType, (RenderObject *)object));
}

void Group::InsertObject(Cube *object)
{
    objects.push_back(std::make_pair(ObjectType::CubeType, (Cube *)object));
}

void Group::Update(double time_passed)
{
    for (int i = 0; i < objects.size(); i++)
    {
        objects.at(i).second->Update(time_passed);
    }
}

} // namespace OpenDoorGL
