#ifndef ODGL_GEOMETRIC_OBJECT_H
#define ODGL_GEOMETRIC_OBJECT_H

#include <GLEW/glew.h>


#include <vector>
#include <string>

#include "odgl_RenderObject.h"

namespace OpenDoorGL{

    class GeometricObject : public RenderObject{
        
    public:
        GeometricObject();
        ~GeometricObject();

        void draw(View view);
        
        void setShadersFromString(const char* vert_shader, const char* frag_shader);
        
    protected:

        GLuint _vertexBuffer;
        GLuint _colorBuffer;
        GLuint _textureBuffer;
        GLuint _normalBuffer;

        std::vector<GLfloat> _vertices;
        std::vector<GLfloat> _vertColors;
        std::vector<GLfloat> _textureCoords;
        std::vector<GLfloat> _normals;

        GLuint _programID;
        GLuint _uniformMVP;
        
        std::string _vertShader;
        std::string _fragShader;
    };
}

#endif