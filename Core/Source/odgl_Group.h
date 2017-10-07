#ifndef ODGL_GROUP_H
#define ODGL_GROUP_H

#include <GLEW/glew.h>


#include <vector>
#include <string>

#include "odgl_RenderObject.h"

namespace OpenDoorGL{

    class Group : public RenderObject{
        
    public:
        Group();
        ~Group();
        void draw(View* view);
        void InsertObject(RenderObject* object);

    protected:

        std::vector<RenderObject*> objects;
    };
}

#endif