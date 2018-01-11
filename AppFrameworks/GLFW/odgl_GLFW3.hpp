#ifndef ODGL_GLFW3
#define ODGL_GLFW3

// Include GLEW
#include <GLEW/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "odgl_WindowInterface.h"


namespace OpenDoorGL {
    
    class GLFW3Window : public WindowInterface {
    public:
        ~GLFW3Window();
        int InitWindow(const char* window_name, bool _vsyncEnabled = true);
        
        double RenderFrame();
        double UpdateFrame();

        bool AppRunning();
        void SetView(View* view);
        View* GetView();
        void InsertObject(RenderObject* object);

        void SetEnableFramerate(bool value);
        bool GetEnableFramerate();
    protected:
        void ComputeMatricesFromInputs();
        GLFWwindow* window;

        double _inputTime;
        double _renderTime;
        double _framePrintTime;
        double _startTime;

        int _numFrames;

        bool _frameRateEnabled;
        bool _vsyncEnabled;
    };
    
}


#endif 