
#include "odgl_RenderObject.h"
#include "odgl_ShaderManager.h"

#include <sstream>

namespace OpenDoorGL{

    RenderObject::RenderObject(){

        glGenBuffers(1, &_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        
        glGenBuffers(1, &_colorBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
        
        glGenBuffers(1, &_textureBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, _textureBuffer);
        
        _model = glm::mat4(1.0f);
        
        // Create and compile our GLSL program from the shaders
        _programID = ShaderManager::LoadShadersFromString( ShaderManager::getColorVertShader(), ShaderManager::getColorFragShader());
        _uniformMVP = glGetUniformLocation(_programID, "MVP");
    }
    
    RenderObject::~RenderObject(){
        // Cleanup VBO and shader
        glDeleteBuffers(1, &_vertexBuffer);
        glDeleteBuffers(1, &_colorBuffer);
        glDeleteBuffers(1, &_textureBuffer);
        glDeleteProgram(_programID);
    }
        
    void RenderObject::setShadersFromFile(const char* vert_shader_file, const char* frag_shader_file){
        if(_programID)
            glDeleteProgram(_programID);
        //_programID = LoadShaders( vert_shader_file, frag_shader_file );
    }
    
  
    void RenderObject::translateObject(float x, float y, float z){
        glm::vec3 trans(x,y,z);
        _model = glm::translate(_model, trans);
    }
}