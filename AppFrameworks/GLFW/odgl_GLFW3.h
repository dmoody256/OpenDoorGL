#ifndef ODGL_GLFW3
#define ODGL_GLFW3

// Include GLEW
#include <GLEW/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "odgl_WindowInterface.h"
#include "odgl_Cube.h"

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

int main( void )
{
    
    OpenDoorGL::WindowInterface* mainWindow = new OpenDoorGL::GLFW3Window();
    mainWindow->InitWindow("Test");
    OpenDoorGL::Cube cube;
    cube.setSize(2.5);
    cube.setColor(1.0f, 0.0f, 0.0f);
    
    mainWindow->InsertObject(&cube);
    while(mainWindow->AppRunning()){
        mainWindow->RenderFrame();
    }

    delete mainWindow;
    return 0;
    
}



#endif 