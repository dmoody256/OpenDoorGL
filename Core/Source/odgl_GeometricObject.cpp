
#include <GLEW/glew.h>

#include <sstream>
#include <vector>
#include <string>

#include "odgl_GeometricObject.h"
#include "odgl_ShaderManager.h"

namespace OpenDoorGL{
    GeometricObject::GeometricObject() : RenderObject(){
        
        glGenBuffers(1, &_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        
        glGenBuffers(1, &_colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
        
        glGenBuffers(1, &_textureBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _textureBuffer);
        
        // Create and compile our GLSL program from the shaders
        _programID = ShaderManager::LoadShadersFromString( ShaderManager::getColorVertShader(), ShaderManager::getColorFragShader());
        _uniformMVP = glGetUniformLocation(_programID, "MVP");
    }
    
    GeometricObject::~GeometricObject(){
        // Cleanup VBO and shader
        glDeleteBuffers(1, &_vertexBuffer);
        glDeleteBuffers(1, &_colorBuffer);
        glDeleteBuffers(1, &_textureBuffer);
        glDeleteProgram(_programID);
    }
        
    void GeometricObject::setShadersFromFile(const char* vert_shader_file, const char* frag_shader_file){
        if(_programID)
            glDeleteProgram(_programID);
        //_programID = LoadShaders( vert_shader_file, frag_shader_file );
    }
    
}