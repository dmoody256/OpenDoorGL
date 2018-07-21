#ifndef ODGL_GLFW3
#define ODGL_GLFW3

// Include GLEW
#include <GLEW/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

#include "odgl_WindowInterface.hpp"

namespace OpenDoorGL
{

class ODGL_DLLEXPORT GLFW3Window : public WindowInterface
{
public:
  ~GLFW3Window();
  bool InitWindow(const char *window_name, bool _vsyncEnabled = true, unsigned int width = 1024, unsigned int height = 768);
  void CleanUp();
  double RenderFrame();
  double UpdateFrame();

  bool AppRunning();
  void SetView(View *view);
  View *GetView();
  void InsertObject(RenderObject *object);

  bool CanCreateCoreProfile();

  void SetEnableFramerate(bool value);
  bool GetEnableFramerate();

protected:
  void ComputeMatricesFromInputs();
  GLFWwindow *window;

  double _inputTime;
  double _renderTime;
  double _framePrintTime;
  double _startTime;

  int _numFrames;

  bool _frameRateEnabled;
  bool _vsyncEnabled;

private:
  bool CreateWindow(GLFWwindow *temp_window, const char *window_name, unsigned int width, unsigned int height);
};

} // namespace OpenDoorGL

#endif