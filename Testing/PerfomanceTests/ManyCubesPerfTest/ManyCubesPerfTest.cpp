#define ODGL_LOG_SCOPE

#include <deque>

#include "odgl_Logging.hpp"
#include "odgl_GLFW3.hpp"
#include "odgl_Cube.hpp"
#include "odgl_View.hpp"

#include "main.hpp"

namespace OpenDoorGL
{

static double lastTime = 0;
static double prevTime = 0;
static int blockNumber = 0;
static int frameCounter = 0;
static double FPSTimer = 0;
static std::deque<double> averageFPS;
static View *testView;

double getAverageFps(const std::deque<double> &fps)
{
    double average = 0;
    for (int i = 0; i < fps.size(); i++)
    {
        average += fps.at(i);
    }
    if (fps.size() > 0)
    {
        return average / (double)fps.size();
    }
    else
    {
        return 0;
    }
}

void TestGroup::Update(double time_passed)
{
    //ODGL_SCOPE();
    double deltaTime = time_passed - lastTime;

    float blockSpeed = 0.0001;
    if (time_passed - lastTime > blockSpeed)
    {
        Cube *cube = new Cube();

        cube->setSize(2.0);
        int zrot = blockNumber / 100;
        float y = (5 + zrot) * sin(2.0f * 3.14f * ((float)blockNumber / 100.0f));
        float x = (5 + zrot) * cos(2.0f * 3.14f * ((float)blockNumber / 100.0f));
        float z = 15 * sin(2.0f * 3.14f * ((float)zrot / 100.0f));
        cube->Translate(x, y, z);
        cube->setColor(1.0f * (1 - zrot / 100.0f), zrot / 100.0f, 0.0f);
        this->InsertObject(cube);
        blockNumber++;
        lastTime = time_passed;
    }

    if (FPSTimer > 1)
    {
        if (averageFPS.size() > 5)
        {
            averageFPS.pop_front();
        }
        averageFPS.push_back(frameCounter);
        double currentAverage = getAverageFps(averageFPS);
        std::cout << currentAverage << " FPS" << std::endl;
        frameCounter = 0;
        FPSTimer = 0;
        if (averageFPS.size() >= 5 && currentAverage < 10)
        {
            std::cout << "Got to " << blockNumber << " before 60Hz was hit" << std::endl;
            exit(0);
        }
    }
    else
    {
        frameCounter++;
        FPSTimer += deltaTime;
    }

    deltaTime = time_passed - prevTime;
    double rate = 50.0f / (1 + time_passed * time_passed);
    testView->position += glm::vec3(rate * deltaTime, rate * deltaTime, rate * deltaTime);
    prevTime = time_passed;
}

WindowInterface *TestWindow::SetupTest()
{

    WindowInterface *mainWindow = new GLFW3Window();
    mainWindow->InitWindow("Stacked Cube Test", false);

    testView = new View();
    testView->view = glm::lookAt(
        glm::vec3(4, 3, 3), // Camera is at (4,3,3), in World Space
        glm::vec3(0, 0, 0), // and looks at the origin
        glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
    );
    testView->proj = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100000.0f);
    testView->position = glm::vec3(4, 3, 3);
    testView->verticalAngle = -0.6f;
    testView->horizontalAngle = 4.0f;

    TestGroup *testGroup = new TestGroup();

    Cube *cube1 = new Cube();
    cube1->setColor(1.0f, 0.0f, 0.0f);
    cube1->setSize(2.0);
    cube1->Translate(1, 1, 0);
    testGroup->InsertObject(cube1);

    mainWindow->InsertObject(testGroup);
    mainWindow->SetView(testView);

    return mainWindow;
}
} // namespace OpenDoorGL