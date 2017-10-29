#include "odgl_GLFW3.h"
#include "odgl_Cube.h"
#include "odgl_View.h"

#include "main.h"

namespace OpenDoorGL{

    WindowInterface* TestWindow::SetupTest(){

        WindowInterface* mainWindow = new GLFW3Window();
        mainWindow->InitWindow("Stacked Cube Test");
    
        View* testView = new View();
        testView->view = glm::lookAt(
            glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        testView->proj = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        testView->position = glm::vec3(4,3,3);
        testView->verticalAngle = -0.6f;
        testView->horizontalAngle = 4.0f;
    
        Cube* cube1 = new Cube();
        cube1->setColor(1.0f, 0.0f, 0.0f);
        cube1->setSize(1.0);
        Cube* cube2 = new Cube();
        cube2->setColor(0.0f, 1.0f, 0.0f);
        cube2->setSize(1.0);
        cube2->Translate(0.0f, 1.0f, 0.0f);
        Cube* cube3 = new Cube();
        cube3->setColor(0.0f, 0.0f, 1.0f);
        cube3->setSize(2.0);
        cube3->Translate(1.5f, 0.5f, -0.5f);
    
        mainWindow->SetView(testView);
        mainWindow->InsertObject(cube1);
        mainWindow->InsertObject(cube2);
        mainWindow->InsertObject(cube3);

        return mainWindow;
    }
}