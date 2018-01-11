
#include <GLEW/glew.h>

#include <sstream>
#include <vector>
#include <string>

#include "odgl_Include.hpp"
#include "odgl_GeometricObject.hpp"
#include "odgl_ShaderManager.hpp"

namespace OpenDoorGL{
    GeometricObject::GeometricObject() : RenderObject(){
        
        GL_CHECK ( glGenBuffers(1, &_vertexBuffer) );
        GL_CHECK ( glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer) );
        
        GL_CHECK ( glGenBuffers(1, &_colorBuffer) );
        GL_CHECK ( glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer) );
        
        GL_CHECK ( glGenBuffers(1, &_textureBuffer) );
        GL_CHECK ( glBindBuffer(GL_ARRAY_BUFFER, _textureBuffer) );

        GL_CHECK ( glGenBuffers(1, &_normalBuffer) );
        GL_CHECK ( glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer) );
        
    }
    
    GeometricObject::~GeometricObject(){
        // Cleanup VBO and shader
        GL_CHECK ( glDeleteBuffers(1, &_vertexBuffer) );
        GL_CHECK ( glDeleteBuffers(1, &_colorBuffer) );
        GL_CHECK ( glDeleteBuffers(1, &_textureBuffer) );
        GL_CHECK ( glDeleteBuffers(1, &_normalBuffer) );
        GL_CHECK ( glDeleteProgram(_programID) );
    }
        
    void GeometricObject::setShadersFromString(const char* vert_shader, const char* frag_shader){
        if(_programID)
            GL_CHECK ( glDeleteProgram(_programID) );
        _programID = ShaderManager::LoadShadersFromString( vert_shader, vert_shader );
    }
    
}