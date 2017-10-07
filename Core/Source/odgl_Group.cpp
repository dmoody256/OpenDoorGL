
#include <GLEW/glew.h>


#include <vector>
#include <string>

#include "odgl_Group.h"

namespace OpenDoorGL{

  
    Group::Group(){

    }
    Group::~Group(){

    }

    void Group::draw(View* view){
        for(int i = 0; i < objects.size(); i++){
            objects.at(i)->draw(view);
        }
    }
    void Group::InsertObject(RenderObject* object){
        objects.push_back(object);
    }

}
