#ifndef ODGL_GEOMETRIC_OBJECT_H
#define ODGL_GEOMETRIC_OBJECT_H

#include <GL/glew.h>


#include <vector>
#include <string>

#include "odgl_RenderObject.h"

namespace OpenDoorGL{

    class GeometricObject : public RenderObject{
        
    public:
        GeometricObject();
        ~GeometricObject();

        void draw(View view);
        void setShadersFromFile(const char* vert_shader_file, const char* frag_shader_file);
        void setShadersFromString(const char* vert_shader, const char* frag_shader);
        
    protected:

        GLuint _vertexBuffer;
        GLuint _colorBuffer;
        GLuint _textureBuffer;
        
        std::vector<GLfloat> _vertices;
        std::vector<GLfloat> _vertColors;
        std::vector<GLfloat> _textureCoords;

        GLuint _programID;
        GLuint _uniformMVP;
        
        std::string _vertShader;
        std::string _fragShader;
    };
}

#endif