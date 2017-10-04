#ifndef ODGL_IMAGE_H
#define ODGL_IMAGE_H

#include <GL/glew.h>

namespace OpenDoorGL{

    class Image{
    public:
        Image();
        ~Image();

        static GLuint LoadImageFromFile(const char* filePath);

        static GLuint LoadBMPFromFile(const char* filePath);

    };



}




#endif