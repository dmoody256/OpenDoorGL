#include "odgl_GLFW3.hpp"
#include "odgl_Button.hpp"
#include "odgl_View.hpp"

#include "main.hpp"

namespace OpenDoorGL{

    WindowInterface* TestWindow::SetupTest(){
        WindowInterface* mainWindow = new GLFW3Window();
        mainWindow->InitWindow("Test", false);

        View* testView = new View();
        testView->view = glm::lookAt(
            glm::vec3(0,0,-3), // Camera is at (4,3,3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        testView->proj = glm::ortho(0.0f, 2.0f, 2.0f, 0.0f, 0.1f, 100.0f);
        //testView->proj = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        testView->position = glm::vec3(0,0,-3);
        testView->_orthographic = true;
        testView->verticalAngle = 0.0f;
        testView->horizontalAngle = 0.0f;

        Button* button = new Button();
        button->setTexture("resources/ButtonTexture.bmp");
        button->setSize(1.0, 1.0);
        
        mainWindow->SetView(testView);
        mainWindow->InsertObject(button);
        return mainWindow;
    }
    
}