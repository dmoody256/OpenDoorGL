#ifndef ODGL_WINDOW_INTERFACE
#define ODGL_WINDOW_INTERFACE

#include "odgl_Include.hpp"
#include "odgl_Group.hpp"
#include "odgl_View.hpp"

namespace OpenDoorGL
{

class ODGL_DLLEXPORT WindowInterface
{
public:
  virtual bool InitWindow(const char *window_name, bool _vsyncEnabled = true, unsigned int width = 1024, unsigned int height = 768) = 0;
  virtual void CleanUp() = 0;
  virtual double RenderFrame() = 0;
  virtual double UpdateFrame() = 0;
  virtual bool AppRunning() = 0;
  virtual void SetView(View *view) = 0;
  virtual View *GetView() = 0;
  virtual void InsertObject(RenderObject *object) = 0;

  virtual bool CanCreateCoreProfile() = 0;

  virtual void SetEnableFramerate(bool value) = 0;
  virtual bool GetEnableFramerate() = 0;

protected:
  Group *topGroup;
  View *_currentView;
  bool _appRunning;
};

} // namespace OpenDoorGL

#endif