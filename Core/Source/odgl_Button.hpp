#ifndef ODGL_BUTTON_H
#define ODGL_BUTTON_H

#include <GLEW/glew.h>
#include <string>

#include "odgl_GeometricObject.hpp"
#include "odgl_Image.hpp"

namespace OpenDoorGL
{

class Button : public GeometricObject
{
  public:
    Button();
    ~Button();

    void setTexture(const char *filepath);
    void setSize(float width, float height);
    
    float getWidth();
    float getHeight();

    void draw(View *view);

  protected:
    float _width;
    float _height;
    Image *_buttonTexture;
    GLuint _uniformTexture;
};
}

#endif