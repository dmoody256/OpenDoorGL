#include "odgl_Cube.hpp"

#include "odgl_Image.hpp"
#include "odgl_ShaderManager.hpp"
#include "odgl_Light.hpp"
#include "odgl_View.hpp"
#include "odgl_Vector.hpp"
#include "odgl_Group.hpp"

#include <glm/gtc/type_ptr.hpp>

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
      lightEnabled(lightEnabled),
      _outline(true),
      _R(0.0f),
      _G(0.0f),
      _B(0.0f)
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
        _programID = ShaderManager::LoadShadersFromString(
            ShaderManager::getColorLightVertShader(),
            ShaderManager::getColorLightFragShader());
        GL_CHECK(_uniformV = glGetUniformLocation(_programID, "V"));
        GL_CHECK(_uniformM = glGetUniformLocation(_programID, "M"));
        GL_CHECK(_uniformLight = glGetUniformLocation(_programID, "LightPosition_worldspace"));
        GL_CHECK(_uniformLightColor = glGetUniformLocation(_programID, "LightColor"));
        GL_CHECK(_uniformLightPower = glGetUniformLocation(_programID, "LightPower"));
    }
    else
    {
        _programID = ShaderManager::LoadShadersFromString(ShaderManager::getColorVertShader(), ShaderManager::getColorFragShader());
    }
    GL_CHECK(_uniformCubeColor = glGetUniformLocation(_programID, "cubeColor"));
    GL_CHECK(_uniformMVP = glGetUniformLocation(_programID, "MVP"));
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

Vector Cube::getCenterPoint()
{

    const float *data = (const float *)glm::value_ptr(_model);
    return Vector(data[12], data[13], data[14]);
}

Vector Cube::getMaxBounds()
{
    const float *data = (const float *)glm::value_ptr(_model);
    return Vector(
        data[12] + (_size / 2.0f),
        data[13] + (_size / 2.0f),
        data[14] + (_size / 2.0f));
}
Vector Cube::getMinBounds()
{
    const float *data = (const float *)glm::value_ptr(_model);
    return Vector(
        data[12] - (_size / 2.0f),
        data[13] - (_size / 2.0f),
        data[14] - (_size / 2.0f));
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

void Cube::setOutline(bool enabled)
{
    _outline = enabled;
}

void Cube::setSize(float size)
{
    _size = size;
    _vertices.resize(0, 0);
    _normals.resize(0, 0);

    // Near Lower Triangle
    float nearsize = 2.0f;
    // 1
    // | ^
    // |  \ 
    // \/  \ 
    // 2--->3
    //
    // Vert 1 /////////////////////////////
    _vertices.push_back(-size / nearsize);
    _vertices.push_back(size / nearsize);
    _vertices.push_back(-size / nearsize);
    // Vert 2 /////////////////////////////
    _vertices.push_back(-size / nearsize);
    _vertices.push_back(-size / nearsize);
    _vertices.push_back(-size / nearsize);
    // Vert 3 /////////////////////////////
    _vertices.push_back(size / nearsize);
    _vertices.push_back(-size / nearsize);
    _vertices.push_back(-size / nearsize);

    // Near Upper Triangle
    // 3<---2
    //  \   ^
    //   \  |
    //    \ |
    //     >1
    //
    // Vert 1 /////////////////////////////
    _vertices.push_back(size / nearsize);
    _vertices.push_back(-size / nearsize);
    _vertices.push_back(-size / nearsize);
    // Vert 2 /////////////////////////////
    _vertices.push_back(size / nearsize);
    _vertices.push_back(size / nearsize);
    _vertices.push_back(-size / nearsize);
    // Vert 3 /////////////////////////////
    _vertices.push_back(-size / nearsize);
    _vertices.push_back(size / nearsize);
    _vertices.push_back(-size / nearsize);

    // Near Face normals
    for (int i = 0; i < 6; i++)
    {
        _normals.push_back(0.0f);
        _normals.push_back(0.0f);
        _normals.push_back(-1.0f);
    }

    // Far Lower Triangle
    float farsize = 2.0f;
    // 1
    // | ^
    // |  \ 
    // \/  \ 
    // 2--->3
    //
    // Vert 1 /////////////////////////////
    _vertices.push_back(size / farsize);
    _vertices.push_back(size / farsize);
    _vertices.push_back(size / farsize);
    // Vert 2 /////////////////////////////
    _vertices.push_back(size / farsize);
    _vertices.push_back(-size / farsize);
    _vertices.push_back(size / farsize);
    // Vert 3 /////////////////////////////
    _vertices.push_back(-size / farsize);
    _vertices.push_back(-size / farsize);
    _vertices.push_back(size / farsize);

    // Far Upper Triangle
    // 3<---2
    //  \   ^
    //   \  |
    //    \ |
    //     >1
    // Vert 1 /////////////////////////////
    _vertices.push_back(-size / farsize);
    _vertices.push_back(-size / farsize);
    _vertices.push_back(size / farsize);
    // Vert 2 /////////////////////////////
    _vertices.push_back(-size / farsize);
    _vertices.push_back(size / farsize);
    _vertices.push_back(size / farsize);
    // Vert 3 /////////////////////////////
    _vertices.push_back(size / farsize);
    _vertices.push_back(size / farsize);
    _vertices.push_back(size / farsize);

    // Far Face Normals
    for (int i = 0; i < 6; i++)
    {
        _normals.push_back(0.0f);
        _normals.push_back(0.0f);
        _normals.push_back(1.0f);
    }

    // Right Lower Triangle
    float rightsize = 2.0f;
    // 1
    // | ^
    // |  \ 
    // \/  \ 
    // 2--->3
    //
    // Vert 1 /////////////////////////////
    _vertices.push_back(size / rightsize);
    _vertices.push_back(size / rightsize);
    _vertices.push_back(-size / rightsize);
    // Vert 2 /////////////////////////////
    _vertices.push_back(size / rightsize);
    _vertices.push_back(-size / rightsize);
    _vertices.push_back(-size / rightsize);
    // Vert 3 /////////////////////////////
    _vertices.push_back(size / rightsize);
    _vertices.push_back(-size / rightsize);
    _vertices.push_back(size / rightsize);

    // Right Upper Triangle
    // 3<---2
    //  \   ^
    //   \  |
    //    \ |
    //     >1
    //
    // Vert 1 /////////////////////////////
    _vertices.push_back(size / rightsize);
    _vertices.push_back(-size / rightsize);
    _vertices.push_back(size / rightsize);
    // Vert 2 /////////////////////////////
    _vertices.push_back(size / rightsize);
    _vertices.push_back(size / rightsize);
    _vertices.push_back(size / rightsize);
    // Vert 3 /////////////////////////////
    _vertices.push_back(size / rightsize);
    _vertices.push_back(size / rightsize);
    _vertices.push_back(-size / rightsize);

    // Right Face Normals
    for (int i = 0; i < 6; i++)
    {
        _normals.push_back(1.0f);
        _normals.push_back(0.0f);
        _normals.push_back(0.0f);
    }

    // Left Lower Triangle
    float leftsize = 2.0f;
    // 1
    // | ^
    // |  \ 
    // \/  \ 
    // 2--->3
    //
    // Vert 1 /////////////////////////////
    _vertices.push_back(-size / leftsize);
    _vertices.push_back(-size / leftsize);
    _vertices.push_back(size / leftsize);
    // Vert 2 /////////////////////////////
    _vertices.push_back(-size / leftsize);
    _vertices.push_back(size / leftsize);
    _vertices.push_back(size / leftsize);
    // Vert 3 /////////////////////////////
    _vertices.push_back(-size / leftsize);
    _vertices.push_back(size / leftsize);
    _vertices.push_back(-size / leftsize);

    // Right Upper Triangle
    // 3<---2
    //  \   ^
    //   \  |
    //    \ |
    //     >1
    //
    // Vert 1 /////////////////////////////
    _vertices.push_back(-size / leftsize);
    _vertices.push_back(size / leftsize);
    _vertices.push_back(-size / leftsize);
    // Vert 2 /////////////////////////////
    _vertices.push_back(-size / leftsize);
    _vertices.push_back(-size / leftsize);
    _vertices.push_back(-size / leftsize);
    // Vert 3 /////////////////////////////
    _vertices.push_back(-size / leftsize);
    _vertices.push_back(-size / leftsize);
    _vertices.push_back(size / leftsize);

    // Left Face Normals
    for (int i = 0; i < 6; i++)
    {
        _normals.push_back(-1.0f);
        _normals.push_back(0.0f);
        _normals.push_back(0.0f);
    }

    // Top Lower Triangle
    float topsize = 2.0f;
    _vertices.push_back(-size / topsize);
    _vertices.push_back(size / topsize);
    _vertices.push_back(-size / topsize);
    _vertices.push_back(-size / topsize);
    _vertices.push_back(size / topsize);
    _vertices.push_back(size / topsize);
    _vertices.push_back(size / topsize);
    _vertices.push_back(size / topsize);
    _vertices.push_back(size / topsize);

    // Top Upper Triangle
    _vertices.push_back(-size / topsize);
    _vertices.push_back(size / topsize);
    _vertices.push_back(-size / topsize);
    _vertices.push_back(size / topsize);
    _vertices.push_back(size / topsize);
    _vertices.push_back(size / topsize);
    _vertices.push_back(size / topsize);
    _vertices.push_back(size / topsize);
    _vertices.push_back(-size / topsize);

    for (int i = 0; i < 6; i++)
    {
        _normals.push_back(0.0f);
        _normals.push_back(1.0f);
        _normals.push_back(0.0f);
    }

    // Bot Lower Triangle
    float botsize = 2.0f;
    _vertices.push_back(-size / botsize);
    _vertices.push_back(-size / botsize);
    _vertices.push_back(-size / botsize);
    _vertices.push_back(-size / botsize);
    _vertices.push_back(-size / botsize);
    _vertices.push_back(size / botsize);
    _vertices.push_back(size / botsize);
    _vertices.push_back(-size / botsize);
    _vertices.push_back(size / botsize);

    // Bot Upper Triangle
    _vertices.push_back(-size / botsize);
    _vertices.push_back(-size / botsize);
    _vertices.push_back(-size / botsize);
    _vertices.push_back(size / botsize);
    _vertices.push_back(-size / botsize);
    _vertices.push_back(size / botsize);
    _vertices.push_back(size / botsize);
    _vertices.push_back(-size / botsize);
    _vertices.push_back(-size / botsize);

    for (int i = 0; i < 6; i++)
    {
        _normals.push_back(0.0f);
        _normals.push_back(-1.0f);
        _normals.push_back(0.0f);
    }

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
    if (visible)
    {

        glm::mat4 renderModel;
        if (parent)
        {
            renderModel = parent->getModel() * _model;
        }

        GL_CHECK(glUseProgram(_programID));

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glm::mat4 MVP = view->proj * view->view * renderModel;
        GL_CHECK(glUniformMatrix4fv(_uniformMVP, 1, GL_FALSE, &MVP[0][0]));
        GL_CHECK(glUniform3f(_uniformCubeColor, _R, _G, _B));

        if (lightEnabled)
        {
            // Send our transformation to the currently bound shader,
            // in the "MVP" uniform
            GL_CHECK(glUniformMatrix4fv(_uniformM, 1, GL_FALSE, &renderModel[0][0]));
            GL_CHECK(glUniformMatrix4fv(_uniformV, 1, GL_FALSE, &view->view[0][0]));
            if (lights.size() == 0)
            {
                GL_CHECK(glUniform3f(_uniformLight, 0, 0, 0));
                GL_CHECK(glUniform3f(_uniformLightColor, 0, 0, 0));
                GL_CHECK(glUniform1f(_uniformLightPower, 0));
            }
            else
            {
                GL_CHECK(glUniform3f(_uniformLight, lights.at(0)->position.x, lights.at(0)->position.y, lights.at(0)->position.z));
                GL_CHECK(glUniform3f(_uniformLightColor, lights.at(0)->color.x, lights.at(0)->color.y, lights.at(0)->color.z));
                GL_CHECK(glUniform1f(_uniformLightPower, lights.at(0)->power));
            }
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

        if (lightEnabled)
        {
            // 3rd attribute buffer : normals
            GL_CHECK(glEnableVertexAttribArray(1));
            GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer));
            GL_CHECK(glVertexAttribPointer(
                1,        // attribute
                3,        // size
                GL_FLOAT, // type
                GL_FALSE, // normalized?
                0,        // stride
                (void *)0 // array buffer offset
                ));
        }
        a

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
        if (_outline)
        {
            GL_CHECK(glUniform3f(_uniformCubeColor, 0.0f, 0.0f, 0.0f));

            GL_CHECK(glDrawArrays(GL_LINE_STRIP, 0, 3));
            GL_CHECK(glDrawArrays(GL_LINE_STRIP, 3, 3));
            GL_CHECK(glDrawArrays(GL_LINE_STRIP, 6, 3));
            GL_CHECK(glDrawArrays(GL_LINE_STRIP, 9, 3));
            GL_CHECK(glDrawArrays(GL_LINES, 13, 2));
            GL_CHECK(glDrawArrays(GL_LINES, 16, 2));
            GL_CHECK(glDrawArrays(GL_LINES, 19, 2));
            GL_CHECK(glDrawArrays(GL_LINES, 22, 2));
        }

        GL_CHECK(glDisableVertexAttribArray(0));
        GL_CHECK(glDisableVertexAttribArray(1));
    }
} // namespace OpenDoorGL
} // namespace OpenDoorGL