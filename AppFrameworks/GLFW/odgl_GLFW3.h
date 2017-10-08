#ifndef ODGL_GLFW3
#define ODGL_GLFW3

// Include GLEW
#include <GLEW/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "odgl_WindowInterface.h"


namespace OpenDoorGL {
    
    class GLFW3Window : public WindowInterface {
    public:

        int InitWindow(const char* window_name);
        void RenderFrame();
        bool AppRunning();
        void InsertObject(RenderObject* object);
    protected:
        GLFWwindow* window;
    };
    
}


#endif 