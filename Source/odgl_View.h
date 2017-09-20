#ifndef ODGL_VIEW_H
#define ODGL_VIEW_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace OpenDoorGL{

    class View{
    public:
        View();
        ~View();

        glm::mat4 proj;
        glm::mat4 view;
        
    };


}


#endif