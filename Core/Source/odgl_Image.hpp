#ifndef ODGL_IMAGE_H
#define ODGL_IMAGE_H

#include "odgl_Include.hpp"

#include <GLEW/glew.h>

namespace OpenDoorGL
{

class ODGL_DLLEXPORT Image
{
  public:
    Image();
    Image(const char *filepath);
    ~Image();

    GLuint LoadImageFromFile(const char *filePath);
    GLuint LoadBMPFromFile(const char *filePath);
    GLuint LoadDDSFromFile(const char *filepath);

    GLuint GetTextureGPUHandle();

  private:
    GLuint _textureID;

    int _height;
    int _width;
};

} // namespace OpenDoorGL

#endif