
#include "odgl_Cube.h"
#include "odgl_Image.h"

namespace OpenDoorGL{

    Cube::Cube() : GeometricObject(){
        _vertices.resize(6*2*3*3, 0);
        _textureCoords.resize(6*2*3*2,0);

    }

    Cube::~Cube(){
        for(int i =0; i < 6; i++){
            glDeleteTextures(1, &_faceTextures[i]);
        }
    }

    void Cube::setTexture(const char* filepath, float* uvcoords){
        for(int i = 0; i < 6; i++){
            setTextureToFace(filepath, uvcoords, i);
        }
    }

    void Cube::setTextureToFace(const char* filepath, float* uvcoords, int faceID ){



        _faceTextures[faceID] = Image::LoadBMPFromFile(filepath);
       
        _textureCoords.at(faceID*12 + 0) = uvcoords[0];
        _textureCoords.at(faceID*12 + 1) = uvcoords[1];
        _textureCoords.at(faceID*12 + 2) = uvcoords[2];
        _textureCoords.at(faceID*12 + 3) = uvcoords[3];
        _textureCoords.at(faceID*12 + 4) = uvcoords[4];
        _textureCoords.at(faceID*12 + 5) = uvcoords[5];

        _textureCoords.at(faceID*12 + 6) = uvcoords[0];
        _textureCoords.at(faceID*12 + 7) = uvcoords[1];
        _textureCoords.at(faceID*12 + 8) = uvcoords[4];
        _textureCoords.at(faceID*12 + 9) = uvcoords[5];
        _textureCoords.at(faceID*12 + 10) = uvcoords[6];
        _textureCoords.at(faceID*12 + 11) = uvcoords[7];

        glBindBuffer(GL_ARRAY_BUFFER, _textureBuffer);
        glBufferData(GL_ARRAY_BUFFER, _textureCoords.size()*sizeof(float), &_textureCoords.at(0), GL_STATIC_DRAW);
    }
    
    void Cube::setSize(float size){
        _size = size;
        _vertices.resize(0, 0);
        // Near Lower Triangle
        _vertices.push_back(-size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f);
        _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f);
        _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f);

        // Near Upper Triangle
        _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f);
        _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f);
        _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f);

        // Right Lower Triangle
        _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f);
        _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f);
        _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f);

        // Right Upper Triangle
        _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f);
        _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f);
        _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f);

        // Far Lower Triangle
        _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f);
        _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f);
        _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f);

        // Far Upper Triangle
        _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f);
        _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f);
        _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f);

        // Left Lower Triangle
        _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f);
        _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f);
        _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f);

        // Left Upper Triangle
        _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f);
        _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f);
        _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f);

        // Top Lower Triangle
        _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f);
        _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f);
        _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f);

        // Top Upper Triangle
        _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f);
        _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f);
        _vertices.push_back(  size/2.0f); _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f);

        // Bot Lower Triangle
        _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f);
        _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f);
        _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f);

        // Bot Upper Triangle
        _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f);
        _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back(  size/2.0f);
        _vertices.push_back(  size/2.0f); _vertices.push_back( -size/2.0f); _vertices.push_back( -size/2.0f);

        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, _vertices.size()*sizeof(GLfloat), &_vertices.at(0), GL_STATIC_DRAW);
        
    }
    void Cube::setColor(float R, float G, float B){
        for(int i = 0; i < 36; i++){
            _vertColors.push_back(R*i/36);
            _vertColors.push_back(G*i/36);
            _vertColors.push_back(B*i/36);
        }
        glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
        glBufferData(GL_ARRAY_BUFFER, _vertColors.size()*sizeof(float), &_vertColors.at(0), GL_STATIC_DRAW);
    }
    void Cube::setColorToFace(float R, float G, float B){

    }
   
    float Cube::getSize(){
        return _size;
    }
        
    void Cube::draw(View* view){
        
        // Use our shader
        glUseProgram(_programID);

        // Send our transformation to the currently bound shader, 
        // in the "MVP" uniform
        glm::mat4 MVP = view->proj * view->view * _model;
        glUniformMatrix4fv(_uniformMVP, 1, GL_FALSE, &MVP[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer);
        glVertexAttribPointer(
            0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
            3,                  // size
            GL_FLOAT,           // type
            GL_FALSE,           // normalized?
            0,                  // stride
            (void*)0            // array buffer offset
        );

        // 2nd attribute buffer : colors
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer);
        glVertexAttribPointer(
            1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            3,                                // size
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );
        
        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, _textureBuffer);
        glVertexAttribPointer(
            2,                                // attribute. No particular reason for 1, but must match the layout in the shader.
            2,                                // size : U+V => 2
            GL_FLOAT,                         // type
            GL_FALSE,                         // normalized?
            0,                                // stride
            (void*)0                          // array buffer offset
        );

        for(int i = 0; i < 6; i++){
            bool activeTexture = false;
            if(_faceTextures[i] != -1){
                if(!activeTexture){
                    //glActiveTexture(GL_TEXTURE0);
                    //glUniform1i(_uniformTexture, GL_TEXTURE0);
                    activeTexture = true;
                }
                //glBindTexture(GL_TEXTURE_2D, _faceTextures[i]);
                glDrawArrays(GL_TRIANGLES, i*2*3, 2*3);
            }
        }

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);
        glDisableVertexAttribArray(2);
    }
}