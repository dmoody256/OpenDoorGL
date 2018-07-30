#include "odgl_Include.hpp"

#include <cstdlib>
#include <iostream>
#include <iomanip>

namespace OpenDoorGL
{
#if defined(ODGL_DEBUG)
void CheckOpenGLError(const char *stmt, const char *fname, int line)
{
    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
    {
        std::cout << "OpenGL error 0x" << std::setfill('0') << std::setw(8) << std::hex << err << std::dec << ": " << gluErrorString(err) << ", at " << fname << ":" << line << " - for " << stmt << std::endl;
        abort();
    }
}
#endif
} // namespace OpenDoorGL