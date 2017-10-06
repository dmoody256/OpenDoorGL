#ifndef ODGL_GLFW3
#define ODGL_GLFW3

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "odgl_WindowInterface.h"

namespace OpenDoorGL {
    
    class GLFW3Window : public WindowInterface {
    public:

        int InitWindow(const char* window_name);
        void RenderFrame();
        bool AppRunning();
    protected:
        GLFWwindow* window;
    };
    
}

int main( void )
{
    
    OpenDoorGL::WindowInterface* mainWindow = new OpenDoorGL::GLFW3Window();
    mainWindow->InitWindow("Test");
    
    while(mainWindow->AppRunning()){
        mainWindow->RenderFrame();
    }

    delete mainWindow;
    return 0;
    
}



#endif 