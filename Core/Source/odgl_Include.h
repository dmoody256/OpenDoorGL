#ifndef ODGL_INCLUDE
#define ODGL_INCLUDE

#include <GLEW/glew.h>

namespace OpenDoorGL{

// Debug print is useful for quickly debugging
// output for things that may happen over time
#ifdef ODGL_DEBUG
#define ODGL_DEBUG_PRINT(x) x
#else
#define ODGL_DEBUG_PRINT(x)
#endif

void CheckOpenGLError(const char* stmt, const char* fname, int line);

#ifdef ODGL_DEBUG
    #define GL_CHECK(stmt) do { \
            stmt; \
            CheckOpenGLError(#stmt, __FILE__, __LINE__); \
        } while (0)
#else
    #define GL_CHECK(stmt) stmt
#endif

}

#endif