#ifndef ODGL_RENDER_OBJECT_H
#define ODGL_RENDER_OBJECT_H

#include <glm/glm.hpp>

#include "odgl_View.h"

namespace OpenDoorGL{

    class RenderObject {
        
    public:
        RenderObject();
        ~RenderObject();

        void Translate(float x, float y, float z);
        virtual void draw(View* view) = 0;
    protected:

        glm::mat4 _model;
    };
}

#endif