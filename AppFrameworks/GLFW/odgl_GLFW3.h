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
        ~GLFW3Window();
        int InitWindow(const char* window_name);
        void RenderFrame();
        bool AppRunning();
        void SetView(View* view);
        void InsertObject(RenderObject* object);
    protected:
        GLFWwindow* window;
    };
    
}


#endif 