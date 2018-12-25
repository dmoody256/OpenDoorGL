#ifndef ODGL_CUBE_H
#define ODGL_CUBE_H

#include <GLEW/glew.h>
#include <string>

#include "odgl_Include.hpp"
#include "odgl_GeometricObject.hpp"
#include "odgl_Image.hpp"
#include "odgl_Light.hpp"

namespace OpenDoorGL
{

class ODGL_DLLEXPORT Cube : public GeometricObject
{
public:
  Cube();
  explicit Cube(bool initGL, bool lightEnabled = false);
  ~Cube();

  void setTexture(const char *filepath, float *uvcoords);
  void setTextureToFace(const char *filepath, float *uvcoords, int faceID);

  void setColor(float R, float G, float B);
  void setColorToFace(float R, float G, float B);

  void setSize(float size);
  float getSize();

  void setOutline(bool enabled);
  glm::vec3 getPosition();

  void draw(View *view) override;
  std::vector<Light *> lights;

protected:
  float _size;
  Image *_faceTextures[6];
  GLuint _uniformTexture;
  GLuint _uniformCubeColor;
  GLuint _uniformLightColor;
  GLuint _uniformLightPower;
  bool lightEnabled;
  bool _outline;
  unsigned char _R;
  unsigned char _G;
  unsigned char _B;

private:
  void InitGL();
};
} // namespace OpenDoorGL

#endif