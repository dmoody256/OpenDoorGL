
#include <GLEW/glew.h>


#include <vector>
#include <string>

#include "odgl_Group.hpp"

namespace OpenDoorGL{

  
    Group::Group(){
        
    }
    Group::~Group(){
        for(int i = 0; i < objects.size(); i++){
            delete objects.at(i);
        }
    }

    void Group::draw(View* view){
        for(int i = 0; i < objects.size(); i++){
            objects.at(i)->draw(view);
        }
    }
    
    void Group::InsertObject(RenderObject* object){
        objects.push_back(object);
    }

    void Group::Update(double time_passed){
        for(int i = 0; i < objects.size(); i++){
            objects.at(i)->Update(time_passed);
        }
    }

}
