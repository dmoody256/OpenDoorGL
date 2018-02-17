#ifndef ODGL_RENDER_OBJECT_H
#define ODGL_RENDER_OBJECT_H

#include <glm/glm.hpp>

#include "odgl_View.hpp"

namespace OpenDoorGL{

    class RenderObject {
        
    public:
        RenderObject();
        ~RenderObject();

        void Translate(float x, float y, float z);
        virtual void draw(View* view) = 0;
        virtual void Update(double time_passed);
    protected:

        glm::mat4 _model;
    };
}

#endif