#ifndef ODGL_CUBE_H
#define ODGL_CUBE_H

#include <GLEW/glew.h>
#include <string>

#include "odgl_Include.hpp"
#include "odgl_GeometricObject.hpp"
#include "odgl_Image.hpp"

namespace OpenDoorGL
{

class ODGL_DLLEXPORT Cube : public GeometricObject
{
public:
  Cube();
  ~Cube();

  void setTexture(const char *filepath, float *uvcoords);
  void setTextureToFace(const char *filepath, float *uvcoords, int faceID);

  void setColor(float R, float G, float B);
  void setColorToFace(float R, float G, float B);

  void setSize(float size);
  float getSize();

  void draw(View *view);

protected:
  float _size;
  Image *_faceTextures[6];
  GLuint _uniformTexture;
};
} // namespace OpenDoorGL

#endif