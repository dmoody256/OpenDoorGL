#ifndef ODGL_MODEL_LOADER
#define ODGL_MODEL_LOADER

#include "odgl_Include.hpp"
#include "odgl_Model.hpp"

namespace OpenDoorGL
{

class ODGL_DLLEXPORT ModelLoader
{

  public:
    static Model *LoadOBJFile(const char *path);
};

} // namespace OpenDoorGL

#endif