#include "odgl_View.h"


namespace OpenDoorGL{

    View::View(){
        // Initial position : on +Z
        position = glm::vec3( 0, 0, 5 ); 
        // Initial horizontal angle : toward -Z
        horizontalAngle = 3.14f;
        // Initial vertical angle : none
        verticalAngle = 0.0f;
        // Initial Field of View
        initialFoV = 45.0f;

        speed = 3.0f; // 3 units / second
        mouseSpeed = 0.005f;
        
    }

    View::~View(){


    }




}