#ifndef ODGL_TESTFRAMEWORK
#define ODGL_TESTFRAMEWORK

#include "odgl_WindowInterface.hpp"

#include <chrono>
#include <iostream>

namespace OpenDoorGL
{

class TestWindow
{

  public:
    static OpenDoorGL::WindowInterface *SetupTest();
};

class TestGroup : public Group
{

  public:
    void Update(double time_passed);
};
} // namespace OpenDoorGL

int main(void)
{
    OpenDoorGL::WindowInterface *mainWindow = OpenDoorGL::TestWindow::SetupTest();

    float numFrames = 0;
    std::chrono::high_resolution_clock::time_point t1, t2;
    t1 = std::chrono::high_resolution_clock::now();

    while (mainWindow->AppRunning())
    {
        numFrames++;
        mainWindow->UpdateFrame();
        mainWindow->RenderFrame();
    }

    t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
    std::cout << time_span.count() / numFrames << std::endl;

    mainWindow->CleanUp();
}

#endif