#include "odgl_Include.hpp"
#include "odgl_Cube.hpp"
#include "odgl_Image.hpp"
#include "odgl_ShaderManager.hpp"

namespace OpenDoorGL
{

Cube::Cube()
    : Cube(true)
{
}

Cube::Cube(bool initGL, bool lightEnabled)
    : GeometricObject(initGL),
      _size(0),
      _uniformTexture(0),
      _uniformCubeColor(0),
      lightEnabled(lightEnabled)
{

    if (initGL)
    {
        InitGL();
    }
    _vertices.resize(6 * 2 * 3 * 3, 0);
    _textureCoords.resize(6 * 2 * 3 * 2, 0);

    for (int i = 0; i < 6; i++)
    {
        _faceTextures[i] = NULL;
    }
}

void Cube::InitGL()
{
    _glInitialized = true;
    // Create and compile our GLSL program from the shaders
    if (lightEnabled)
    {
        _programID = ShaderManager::LoadShadersFromString(ShaderManager::getColorLightVertShader(), ShaderManager::getColorLightFragShader());
        GL_CHECK(_uniformMVP = glGetUniformLocation(_programID, "MVP"));
        GL_CHECK(_uniformV = glGetUniformLocation(_programID, "V"));
        GL_CHECK(_uniformM = glGetUniformLocation(_programID, "M"));
        GL_CHECK(_uniformCubeColor = glGetUniformLocation(_programID, "cubeColor"));
        GL_CHECK(_uniformLight = glGetUniformLocation(_programID, "LightPosition_worldspace"));
    }
    else
    {
        _programID = ShaderManager::LoadShadersFromString(ShaderManager::getColorVertShader(), ShaderManager::getColorFragShader());
        GL_CHECK(_uniformMVP = glGetUniformLocation(_programID, "MVP"));
    }
}

Cube::~Cube()
{
    for (int i = 0; i < 6; i++)
    {
        if (_faceTextures[i])
        {
            GLuint texture = _faceTextures[i]->GetTextureGPUHandle();
            GL_CHECK(glDeleteTextures(1, &texture));
            delete _faceTextures[i];
        }
    }
}

void Cube::setTexture(const char *filepath, float *uvcoords)
{
    for (int i = 0; i < 6; i++)
    {
        setTextureToFace(filepath, uvcoords, i);
    }
}

void Cube::setTextureToFace(const char *filepath, float *uvcoords, int faceID)
{

    _faceTextures[faceID] = new Image(filepath);

    _textureCoords.at(faceID * 12 + 0) = uvcoords[0];
    _textureCoords.at(faceID * 12 + 1) = uvcoords[1];
    _textureCoords.at(faceID * 12 + 2) = uvcoords[2];
    _textureCoords.at(faceID * 12 + 3) = uvcoords[3];
    _textureCoords.at(faceID * 12 + 4) = uvcoords[4];
    _textureCoords.at(faceID * 12 + 5) = uvcoords[5];

    _textureCoords.at(faceID * 12 + 6) = uvcoords[0];
    _textureCoords.at(faceID * 12 + 7) = uvcoords[1];
    _textureCoords.at(faceID * 12 + 8) = uvcoords[4];
    _textureCoords.at(faceID * 12 + 9) = uvcoords[5];
    _textureCoords.at(faceID * 12 + 10) = uvcoords[6];
    _textureCoords.at(faceID * 12 + 11) = uvcoords[7];

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _textureBuffer));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, _textureCoords.size() * sizeof(float), &_textureCoords.at(0), GL_STATIC_DRAW));
}

void Cube::setSize(float size)
{
    _size = size;
    _vertices.resize(0, 0);
    _normals.resize(0, 0);

    // Near Lower Triangle
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _normals.push_back(0.0f);
    _normals.push_back(0.0f);
    _normals.push_back(-1.0f);

    // Near Upper Triangle

    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _normals.push_back(0.0f);
    _normals.push_back(0.0f);
    _normals.push_back(1.0f);

    // Right Lower Triangle
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _normals.push_back(1.0f);
    _normals.push_back(0.0f);
    _normals.push_back(0.0f);

    // Right Upper Triangle
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _normals.push_back(1.0f);
    _normals.push_back(0.0f);
    _normals.push_back(0.0f);

    // Far Lower Triangle
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _normals.push_back(0.0f);
    _normals.push_back(0.0f);
    _normals.push_back(-1.0f);

    // Far Upper Triangle
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _normals.push_back(0.0f);
    _normals.push_back(0.0f);
    _normals.push_back(-1.0f);

    // Left Lower Triangle
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _normals.push_back(-1.0f);
    _normals.push_back(0.0f);
    _normals.push_back(0.0f);

    // Left Upper Triangle
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _normals.push_back(-1.0f);
    _normals.push_back(0.0f);
    _normals.push_back(0.0f);

    // Top Lower Triangle
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _normals.push_back(0.0f);
    _normals.push_back(1.0f);
    _normals.push_back(0.0f);

    // Top Upper Triangle
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _normals.push_back(0.0f);
    _normals.push_back(1.0f);
    _normals.push_back(0.0f);

    // Bot Lower Triangle
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _normals.push_back(0.0f);
    _normals.push_back(-1.0f);
    _normals.push_back(0.0f);

    // Bot Upper Triangle
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _vertices.push_back(-size / 2.0f);
    _normals.push_back(0.0f);
    _normals.push_back(1.0f);
    _normals.push_back(0.0f);

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), &_vertices.at(0), GL_STATIC_DRAW));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(GLfloat), &_normals.at(0), GL_STATIC_DRAW));
}

void Cube::setColor(float R, float G, float B)
{
    _R = R;
    _G = G;
    _B = B;
    for (int i = 0; i < 36; i++)
    {
        if (lightEnabled)
        {
            _vertColors.push_back(R * i / 36);
            _vertColors.push_back(G * i / 36);
            _vertColors.push_back(B * i / 36);
        }
        else
        {
            _vertColors.push_back(R);
            _vertColors.push_back(G);
            _vertColors.push_back(B);
        }
    }
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, _vertColors.size() * sizeof(float), &_vertColors.at(0), GL_STATIC_DRAW));
}
void Cube::setColorToFace(float R, float G, float B)
{
}

float Cube::getSize()
{
    return _size;
}

void Cube::draw(View *view)
{

    // Use our shader
    GL_CHECK(glUseProgram(_programID));

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glm::mat4 MVP = view->proj * view->view * _model;
    GL_CHECK(glUniformMatrix4fv(_uniformMVP, 1, GL_FALSE, &MVP[0][0]));

    if (lightEnabled)
    {
        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        GL_CHECK(glUniformMatrix4fv(_uniformM, 1, GL_FALSE, &view->proj[0][0]));
        GL_CHECK(glUniformMatrix4fv(_uniformV, 1, GL_FALSE, &view->view[0][0]));

        glm::vec3 lightPos = view->position;
        GL_CHECK(glUniform3f(_uniformLight, lightPos.x, lightPos.y, lightPos.z));

        GL_CHECK(glUniform3f(_uniformCubeColor, _R, _G, _B));
    }

    // 1rst attribute buffer : vertices
    GL_CHECK(glEnableVertexAttribArray(0));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer));
    GL_CHECK(glVertexAttribPointer(
        0,        // attribute. No particular reason for 0, but must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
        ));

    // 2nd attribute buffer : colors
    GL_CHECK(glEnableVertexAttribArray(1));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _colorBuffer));
    GL_CHECK(glVertexAttribPointer(
        1,        // attribute. No particular reason for 1, but must match the layout in the shader.
        3,        // size
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
        ));

    if (lightEnabled)
    {
        // 3rd attribute buffer : normals
        GL_CHECK(glEnableVertexAttribArray(2));
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer));
        GL_CHECK(glVertexAttribPointer(
            2,        // attribute
            3,        // size
            GL_FLOAT, // type
            GL_FALSE, // normalized?
            0,        // stride
            (void *)0 // array buffer offset
            ));
    }
    for (int i = 0; i < 6; i++)
    {
        //bool activeTexture = false;
        //if (_faceTextures[i] && _faceTextures[i]->GetTextureGPUHandle() != -1)
        //{
        //    if (!activeTexture)
        //    {
        //        //glActiveTexture(GL_TEXTURE0);
        //        //glUniform1i(_uniformTexture, GL_TEXTURE0);
        //        //activeTexture = true;
        //    }
        //    //glBindTexture(GL_TEXTURE_2D, _faceTextures[i]->GetTextureGPUHandle());
        //}
        GL_CHECK(glDrawArrays(GL_TRIANGLES, i * 2 * 3, 2 * 3));
    }

    GL_CHECK(glDisableVertexAttribArray(0));
    GL_CHECK(glDisableVertexAttribArray(1));
    GL_CHECK(glDisableVertexAttribArray(2));
}
}