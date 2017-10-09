#include "odgl_GLFW3.h"

namespace OpenDoorGL{

    int GLFW3Window::InitWindow(const char* window_name){

        // Initialise GLFW
        if( !glfwInit() )
        {
            fprintf( stderr, "Failed to initialize GLFW\n" );
            return -1;
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Open a window and create its OpenGL context
        window = glfwCreateWindow( 1024, 768, window_name, NULL, NULL);
        if( window == NULL ){
            fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);

        // Initialize GLEW
        glewExperimental = true; // Needed for core profile
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to initialize GLEW\n");
            return -1;
        }

        GLuint VertexArrayID;
        glGenVertexArrays(1, &VertexArrayID);
        glBindVertexArray(VertexArrayID);
        
        glfwSwapInterval(1);
        // Enable depth test
        glEnable(GL_DEPTH_TEST);
        // Accept fragment if it closer to the camera than the former one
        glDepthFunc(GL_LESS); 

        // Ensure we can capture the escape key being pressed below
        glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
        _appRunning = true;

        _currentView = new View();
        _currentView->view = glm::lookAt(
            glm::vec3(4,3,3), // Camera is at (4,3,3), in World Space
            glm::vec3(0,0,0), // and looks at the origin
            glm::vec3(0,1,0)  // Head is up (set to 0,-1,0 to look upside-down)
        );
        _currentView->proj = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        topGroup = new Group();
        return 0;
    }
    GLFW3Window::~GLFW3Window(){
       
        delete topGroup;
        delete _currentView;
        // Close OpenGL window and terminate GLFW
        glfwTerminate();    
    }

    void GLFW3Window::SetView(View* view){
        _currentView = view;
    }
    void GLFW3Window::InsertObject(RenderObject* object){
        topGroup->InsertObject(object);
    }
    void GLFW3Window::RenderFrame(){

        topGroup->draw(_currentView);
        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    bool GLFW3Window::AppRunning(){
        if(glfwGetKey(window, GLFW_KEY_ESCAPE ) == GLFW_PRESS || glfwWindowShouldClose(window)){
            _appRunning = false;
        }
        return _appRunning;
    }

}

