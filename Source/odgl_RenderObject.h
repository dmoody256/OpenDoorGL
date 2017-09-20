#ifndef ODGL_RENDER_OBJECT_H
#define ODGL_RENDER_OBJECT_H

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <string>

namespace OpenDoorGL{

    class RenderObject {
        
    public:
        RenderObject();
        ~RenderObject();

        void setShadersFromFile(const char* vert_shader_file, const char* frag_shader_file);
        void setShadersFromString(const char* vert_shader, const char* frag_shader);

        void translateObject(float x, float y, float z);
        
    protected:

        GLuint _vertexBuffer;
        GLuint _colorBuffer;
        GLuint _textureBuffer;
        
        std::vector<GLfloat> _vertices;
        std::vector<GLfloat> _vertColors;
        std::vector<GLfloat> _textureCoords;

        glm::mat4 _model;

        GLuint _programID;
        GLuint _uniformMVP;
        
        std::string _vertShader;
        std::string _fragShader;
    };
}

#endif