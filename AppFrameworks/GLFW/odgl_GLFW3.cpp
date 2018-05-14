#include "odgl_GLFW3.hpp"

namespace OpenDoorGL
{

int GLFW3Window::InitWindow(const char *window_name, bool _vsyncEnabled)
{

    // Initialise GLFW
    if (!glfwInit())
    {
        fprintf(stderr, "Failed to initialize GLFW\n");
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(1024, 768, window_name, NULL, NULL);
    if (window == NULL)
    {
        fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Failed to initialize GLEW\n");
        return -1;
    }

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
    if (_vsyncEnabled)
    {
        glfwSwapInterval(1);
    }
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    /* TODO:
         *
         * fix the orientation of vertices so that I can use backface culling
         * specifically this happens in the cube at this point in time
         *
         */
    //glEnable(GL_CULL_FACE);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    _appRunning = true;
    // Reset mouse position for next frame
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);
    _currentView = new View();
    _currentView->view = glm::lookAt(
        glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    _currentView->proj = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    _currentView->position = glm::vec3(4, 3, 3);
    topGroup = new Group();
    _inputTime = glfwGetTime();
    _renderTime = _inputTime;
    _framePrintTime = _renderTime;
    _startTime = glfwGetTime();
    _numFrames = 0;

    return 0;
}
GLFW3Window::~GLFW3Window()
{

    delete topGroup;
    delete _currentView;
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
}

void GLFW3Window::SetView(View *view)
{
    _currentView = view;
}
View *GLFW3Window::GetView()
{
    return _currentView;
}
void GLFW3Window::InsertObject(RenderObject *object)
{
    topGroup->InsertObject(object);
}

double GLFW3Window::UpdateFrame()
{
    double frameTime = glfwGetTime() - _startTime;
    topGroup->Update(frameTime);
    return frameTime;
}

void GLFW3Window::ComputeMatricesFromInputs()
{

    // Compute time difference between current and last frame
    double currentTime = glfwGetTime();
    float deltaTime = float(currentTime - _inputTime);

    // Get mouse position
    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);

    // Reset mouse position for next frame
    glfwSetCursorPos(window, 1024 / 2, 768 / 2);

    // Compute new orientation
    _currentView->horizontalAngle += _currentView->mouseSpeed * float(1024 / 2 - xpos);
    _currentView->verticalAngle += _currentView->mouseSpeed * float(768 / 2 - ypos);

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
        cos(_currentView->verticalAngle) * sin(_currentView->horizontalAngle),
        sin(_currentView->verticalAngle),
        cos(_currentView->verticalAngle) * cos(_currentView->horizontalAngle));

    // Right vector
    glm::vec3 right = glm::vec3(
        sin(_currentView->horizontalAngle - 3.14f / 2.0f),
        0,
        cos(_currentView->horizontalAngle - 3.14f / 2.0f));

    // Up vector
    glm::vec3 up = glm::cross(right, direction);

    // Move forward
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        _currentView->position += direction * deltaTime * _currentView->speed;
    }
    // Move backward
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        _currentView->position -= direction * deltaTime * _currentView->speed;
    }
    // Strafe right
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        _currentView->position += right * deltaTime * _currentView->speed;
    }
    // Strafe left
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        _currentView->position -= right * deltaTime * _currentView->speed;
    }

    float FoV = _currentView->initialFoV; // - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

    if (_currentView->_orthographic)
    {
        _currentView->proj = glm::ortho(-2.0f, 2.0f, -2.0f, 2.0f, 0.1f, 10000.0f);
    }
    else
    {
        // Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
        _currentView->proj = glm::perspective(glm::radians(FoV), 4.0f / 3.0f, 0.1f, 10000.0f);
    }

    // Camera matrix
    _currentView->view = glm::lookAt(
        _currentView->position,             // Camera is here
        _currentView->position + direction, // and looks here : at the same position, plus "direction"
        up                                  // Head is up (set to 0,-1,0 to look upside-down)
    );

    // For the next frame, the "last time" will be "now"
    _inputTime = currentTime;
}

void GLFW3Window::SetEnableFramerate(bool value)
{
    _frameRateEnabled = value;
}
bool GLFW3Window::GetEnableFramerate()
{
    return _frameRateEnabled;
}

double GLFW3Window::RenderFrame()
{

    ComputeMatricesFromInputs();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    topGroup->draw(_currentView);
    // Swap buffers
    glfwSwapBuffers(window);
    glfwPollEvents();

    double currentTime = glfwGetTime();
    double deltaTime = glfwGetTime() - _renderTime;
    _renderTime = currentTime;

    if (_frameRateEnabled)
    {
        _numFrames++;
        if (currentTime - _framePrintTime >= 1.0)
        { // If last prinf() was more than 1 sec ago
            // printf and reset timer
            printf("%f ms/frame\n", 1000.0 / double(_numFrames));
            _numFrames = 0;
            _framePrintTime += 1.0;
        }
    }

    return deltaTime;
}

bool GLFW3Window::AppRunning()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(window))
    {
        _appRunning = false;
    }
    return _appRunning;
}

} // namespace OpenDoorGL
