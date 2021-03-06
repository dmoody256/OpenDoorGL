#include "odgl_GLFW3.hpp"
#include "odgl_Model.hpp"
#include "odgl_ModelLoader.hpp"
#include "odgl_View.hpp"

#include "main.hpp"

namespace OpenDoorGL{

    WindowInterface* TestWindow::SetupTest(){

        WindowInterface* mainWindow = new GLFW3Window();
        mainWindow->InitWindow("Load Obj Test");
    
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
    
        Model* model = ModelLoader::LoadOBJFile("resources/LoadObjTest.obj");
        Image* image = new Image("resources/LoadObjTest.dds");
        model->SetTexture(image);
        mainWindow->SetView(testView);
        mainWindow->InsertObject(model);

        return mainWindow;
    }
}