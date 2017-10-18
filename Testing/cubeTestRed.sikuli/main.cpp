#include "odgl_GLFW3.h"
#include "odgl_Cube.h"
#include "odgl_View.h"

int main( void )
{
    
    OpenDoorGL::WindowInterface* mainWindow = new OpenDoorGL::GLFW3Window();
    mainWindow->InitWindow("Test");

    OpenDoorGL::View testView;
    testView.view = glm::lookAt(
        glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
        glm::vec3(0,0,0), // and looks at the origin
        glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    testView.proj = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    testView.position = glm::vec3(4,3,3);
    testView.verticalAngle = -0.6f;
    testView.horizontalAngle = 4.0f;
    OpenDoorGL::Cube cube;
    cube.setColor(1.0f, 0.0f, 0.0f);
    cube.setSize(1.0);
    
    mainWindow->SetView(&testView);
    mainWindow->InsertObject(&cube);

    while( mainWindow->AppRunning() ){
        mainWindow->RenderFrame();
    }

    delete mainWindow;
    
    return 0;
    
}