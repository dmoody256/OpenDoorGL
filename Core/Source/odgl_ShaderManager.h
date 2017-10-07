#ifndef ODGL_SHADER_MANAGER_H
#define ODGL_SHADER_MANAGER_H

#include <GLEW/glew.h>

#include <string>

namespace OpenDoorGL{

    class ShaderManager{
    public:
        ShaderManager();
        ~ShaderManager();
        static GLuint LoadShadersFromString(const char* vert_shader, const char* frag_shader);
        static const char* getColorVertShader();
        static const char* getColorFragShader();
    private:
        static std::string colorVertShader;
        static std::string colorFragShader;
    };

}

#endif