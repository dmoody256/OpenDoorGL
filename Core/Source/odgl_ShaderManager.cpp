
#include "odgl_Include.hpp"
#include "odgl_ShaderManager.hpp"
#include "odgl_Shaders.hpp"

#include <sstream>
#include <vector>

namespace OpenDoorGL
{

const char *ShaderManager::GetTextureModelFragShader()
{
    return textureModel_frag.c_str();
}

const char *ShaderManager::GetTextureModelVertShader()
{
    return textureModel_vert.c_str();
}

const char *ShaderManager::getColorFragShader()
{
    return color_frag.c_str();
}

const char *ShaderManager::getColorVertShader()
{
    return color_vert.c_str();
}

const char *ShaderManager::getTextureFragShader()
{
    return texture_frag.c_str();
}

const char *ShaderManager::getTextureVertShader()
{
    return texture_vert.c_str();
}

const char *ShaderManager::getButtonVertShader()
{
    return button_vert.c_str();
}
const char *ShaderManager::getButtonFragShader()
{
    return button_frag.c_str();
}

const char *ShaderManager::getColorLightVertShader()
{
    return colorlight_vert.c_str();
}
const char *ShaderManager::getColorLightFragShader()
{
    return colorlight_frag.c_str();
}

GLuint ShaderManager::LoadShadersFromString(const char *vertShader, const char *fragShader)
{
    GLint Result = GL_FALSE;
    int InfoLogLength;
    // Create the shaders
    GLuint VertexShaderID;
    GL_CHECK(VertexShaderID = glCreateShader(GL_VERTEX_SHADER));
    GLuint FragmentShaderID;
    GL_CHECK(FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER));

    // Compile Vertex Shader
    //printf("Compiling shader : %s\n", vertShader);
    GL_CHECK(glShaderSource(VertexShaderID, 1, &vertShader, NULL));
    GL_CHECK(glCompileShader(VertexShaderID));

    // Check Vertex Shader
    GL_CHECK(glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result));
    GL_CHECK(glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength));
    if (InfoLogLength > 0)
    {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        GL_CHECK(glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]));
        printf("%s\n", &VertexShaderErrorMessage[0]);
    }

    // Compile Fragment Shader
    //printf("Compiling shader : %s\n", fragShader);
    GL_CHECK(glShaderSource(FragmentShaderID, 1, &fragShader, NULL));
    GL_CHECK(glCompileShader(FragmentShaderID));

    // Check Fragment Shader
    GL_CHECK(glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result));
    GL_CHECK(glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength));
    if (InfoLogLength > 0)
    {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        GL_CHECK(glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]));
        printf("%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Link the program
    //printf("Linking program\n");
    GLuint ProgramID;
    GL_CHECK(ProgramID = glCreateProgram());
    GL_CHECK(glAttachShader(ProgramID, VertexShaderID));
    GL_CHECK(glAttachShader(ProgramID, FragmentShaderID));
    GL_CHECK(glLinkProgram(ProgramID));

    // Check the program
    GL_CHECK(glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result));
    GL_CHECK(glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength));
    if (InfoLogLength > 0)
    {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        GL_CHECK(glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]));
        printf("%s\n", &ProgramErrorMessage[0]);
    }

    GL_CHECK(glDetachShader(ProgramID, VertexShaderID));
    GL_CHECK(glDetachShader(ProgramID, FragmentShaderID));

    GL_CHECK(glDeleteShader(VertexShaderID));
    GL_CHECK(glDeleteShader(FragmentShaderID));

    return ProgramID;
}

} // namespace OpenDoorGL
