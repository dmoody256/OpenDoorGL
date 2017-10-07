#ifndef ODGL_WINDOW_INTERFACE
#define ODGL_WINDOW_INTERFACE

#include "odgl_Group.h"
#include "odgl_View.h"

namespace OpenDoorGL{

    class WindowInterface {
    public:
        virtual int InitWindow(const char* window_name) = 0;
        virtual void RenderFrame() = 0;
        virtual bool AppRunning() = 0;
        virtual void InsertObject(RenderObject* object) = 0;
    protected:
        Group* topGroup;
        View* _currentView;
        bool _appRunning;
       
    };

}

#endif