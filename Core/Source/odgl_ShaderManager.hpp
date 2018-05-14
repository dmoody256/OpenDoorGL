#ifndef ODGL_SHADER_MANAGER_H
#define ODGL_SHADER_MANAGER_H

#include "odgl_Include.hpp"

#include <GLEW/glew.h>

#include <string>

namespace OpenDoorGL
{

class ODGL_DLLEXPORT ShaderManager
{
  public:
    ShaderManager();
    ~ShaderManager();
    static GLuint LoadShadersFromString(const char *vert_shader, const char *frag_shader);
    static const char *getColorVertShader();
    static const char *getColorFragShader();
    static const char *getTextureVertShader();
    static const char *getTextureFragShader();
    static const char *GetTextureModelVertShader();
    static const char *GetTextureModelFragShader();

  private:
    static std::string colorVertShader;
    static std::string colorFragShader;
    static std::string textureVertShader;
    static std::string textureFragShader;
    static std::string textureModelVertShader;
    static std::string textureModelFragShader;
};

} // namespace OpenDoorGL

#endif