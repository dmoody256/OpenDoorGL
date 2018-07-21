
#include <GLEW/glew.h>


#include <vector>
#include <string>

#include "odgl_Group.hpp"

namespace OpenDoorGL{

  
    Group::Group(){
        
    }

    Group::~Group(){
        for(int i = 0; i < objects.size(); i++){
            switch(objects.at(i).first)
            {
                case ObjectType::RenderObjectType:
                case ObjectType::UnknownType:
                    delete objects.at(i).second;
                    break;
                case ObjectType::GeometricObjectType:
                    delete (GeometricObject*)objects.at(i).second;
                    break;
                default:
                    delete objects.at(i).second;
                    break;
            }
        }
    }

    void Group::draw(View* view){
        for(int i = 0; i < objects.size(); i++){
            objects.at(i).second->draw(view);
        }
    }
    
    void Group::InsertObject(RenderObject* object){
        objects.push_back(std::make_pair( ObjectType::RenderObjectType, object));
    }

    void Group::InsertObject(GeometricObject* object){
        objects.push_back(std::make_pair( ObjectType::GeometricObjectType, (RenderObject*)object));
    }

    void Group::Update(double time_passed){
        for(int i = 0; i < objects.size(); i++){
            objects.at(i).second->Update(time_passed);
        }
    }

}
