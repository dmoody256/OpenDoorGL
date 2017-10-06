#ifndef ODGL_CUBE_H
#define ODGL_CUBE_H

#include <GL/glew.h>
#include <string>

#include "odgl_GeometricObject.h"

namespace OpenDoorGL{

    class Cube : public GeometricObject{
    public:
        Cube();
        ~Cube();

        void setTexture(const char* filepath, float* uvcoords);
        void setTextureToFace(const char* filepath, float* uvcoords, int faceID);

        void setColor(float R, float G, float B);
        void setColorToFace(float R, float G, float B);

        void setSize(float size);
        float getSize();

        void draw(View* view);

    protected:
        float _size;
        GLuint _faceTextures[6];
        GLuint _uniformTexture;
    };
}

#endif