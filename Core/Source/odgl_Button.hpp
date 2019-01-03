#ifndef ODGL_BUTTON_H
#define ODGL_BUTTON_H

#include <GLEW/glew.h>
#include <string>

#include "odgl_Include.hpp"
#include "odgl_GeometricObject.hpp"
#include "odgl_Image.hpp"

namespace OpenDoorGL
{

class ODGL_DLLEXPORT Button : public GeometricObject
{
public:
  Button();
  explicit Button(bool initGL);
  ~Button();

  void setTexture(const char *filepath);
  void setSize(float width, float height);

  void setColor(const unsigned char R, const unsigned char G, const unsigned char B, const unsigned char A);

  float getWidth();
  float getHeight();

  void draw(View *view) override;

protected:
  float _width;
  float _height;
  GLuint _buttonTexture;
  GLuint _uniformTexture;
};
} // namespace OpenDoorGL

#endif