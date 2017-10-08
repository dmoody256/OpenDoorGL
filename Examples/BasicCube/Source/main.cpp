#include "odgl_GLFW3.h"
#include "odgl_Cube.h"


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