#include "odgl_Include.hpp"
#include "odgl_Button.hpp"
#include "odgl_Image.hpp"
#include "odgl_ShaderManager.hpp"

namespace OpenDoorGL
{

Button::Button()
{
    // Create and compile our GLSL program from the shaders
    _programID = ShaderManager::LoadShadersFromString(ShaderManager::getTextureVertShader(), ShaderManager::getTextureFragShader());
    GL_CHECK(_uniformMVP = glGetUniformLocation(_programID, "MVP"));
    GL_CHECK(_uniformTexture = glGetUniformLocation(_programID, "myTextureSampler"));
    _vertices.resize(2 * 3 * 3, 0);
    _textureCoords.resize(2 * 3 * 2, 0);
    _width = 0.0f;
    _height = 0.0f;
    _buttonTexture = NULL;
}
Button::~Button()
{
    // testing change the source
    if (_buttonTexture)
    {
        GLuint texture = _buttonTexture->GetTextureGPUHandle();
        GL_CHECK(glDeleteTextures(1, &texture));
        delete _buttonTexture;
    }
}

void Button::setTexture(const char *filepath)
{
    _buttonTexture = new Image(filepath);

    _textureCoords.at(0) = 0;
    _textureCoords.at(1) = 0;
    _textureCoords.at(2) = 1;
    _textureCoords.at(3) = 0;
    _textureCoords.at(4) = 1;
    _textureCoords.at(5) = 1;
    _textureCoords.at(6) = 0;
    _textureCoords.at(7) = 0;
    _textureCoords.at(8) = 1;
    _textureCoords.at(9) = 1;
    _textureCoords.at(10) = 0;
    _textureCoords.at(11) = 1;

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _textureBuffer));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, _textureCoords.size() * sizeof(float), &_textureCoords.at(0), GL_STATIC_DRAW));
}

void Button::setSize(float width, float height)
{
    _width = width;
    _height = height;
    // Lower Left Triangle
    _vertices[0] = (-_width / 2.0f);
    _vertices[1] = (-_height / 2.0f);
    _vertices[2] = (0);
    _vertices[3] = (_width / 2.0f);
    _vertices[4] = (-_height / 2.0f);
    _vertices[5] = (0);
    _vertices[6] = (_width / 2.0f);
    _vertices[7] = (_height / 2.0f);
    _vertices[8] = (0);

    // Upper Right Triangle
    _vertices[9] = (-_width / 2.0f);
    _vertices[10] = (-_height / 2.0f);
    _vertices[11] = (0);
    _vertices[12] = (_width / 2.0f);
    _vertices[13] = (_height / 2.0f);
    _vertices[14] = (0);
    _vertices[15] = (-_width / 2.0f);
    _vertices[16] = (_height / 2.0f);
    _vertices[17] = (0);

    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer));
    GL_CHECK(glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), &_vertices.at(0), GL_STATIC_DRAW));
}
float Button::getWidth()
{
    return _width;
}

float Button::getHeight()
{
    return _height;
}

void Button::draw(View *view)
{
    // Use our shader
    GL_CHECK(glUseProgram(_programID));

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform
    glm::mat4 MVP = view->proj * view->view * _model;
    GL_CHECK(glUniformMatrix4fv(_uniformMVP, 1, GL_FALSE, &MVP[0][0]));

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

    // 2nd attribute buffer : UVs
    GL_CHECK(glEnableVertexAttribArray(1));
    GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _textureBuffer));
    GL_CHECK(glVertexAttribPointer(
        1,        // attribute. No particular reason for 1, but must match the layout in the shader.
        2,        // size : U+V => 2
        GL_FLOAT, // type
        GL_FALSE, // normalized?
        0,        // stride
        (void *)0 // array buffer offset
        ));

    // Bind our texture in Texture Unit 0
    GL_CHECK(glActiveTexture(GL_TEXTURE0));
    GL_CHECK(glBindTexture(GL_TEXTURE_2D, _buttonTexture->GetTextureGPUHandle()));
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    GL_CHECK(glUniform1i(_uniformTexture, 0));

    GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, 2 * 3));

    GL_CHECK(glDisableVertexAttribArray(0));
    GL_CHECK(glDisableVertexAttribArray(1));
}
}