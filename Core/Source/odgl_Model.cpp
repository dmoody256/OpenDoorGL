

#include "odgl_Logging.hpp"
#include "odgl_Model.hpp"
#include "odgl_Include.hpp"
#include "odgl_ShaderManager.hpp"

namespace OpenDoorGL
{

Model::Model(bool init_gl) : GeometricObject(init_gl)
{
    if (init_gl)
    {
        InitGL();
    }
}

Model::~Model()
{
    ODGL_SCOPE();

    if (_texture)
    {
        delete _texture;
    }
}

void Model::InitGL()
{
    // Create and compile our GLSL program from the shaders
    _programID = ShaderManager::LoadShadersFromString(ShaderManager::GetTextureModelVertShader(), ShaderManager::GetTextureModelFragShader());
    GL_CHECK(_uniformMVP = glGetUniformLocation(_programID, "MVP"));
    GL_CHECK(_uniformV = glGetUniformLocation(_programID, "V"));
    GL_CHECK(_uniformM = glGetUniformLocation(_programID, "M"));
    GL_CHECK(_uniformSampler1 = glGetUniformLocation(_programID, "myTextureSampler"));
    GL_CHECK(_uniformLight = glGetUniformLocation(_programID, "LightPosition_worldspace"));
}

void Model::SetNormals(int const numNormals, GLfloat const *normals)
{

    for (int i = 0; i < numNormals; i++)
    {
        _normals.push_back(normals[i]);
    }
    if (_glInitialized)
    {
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _normalBuffer));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(float), &_normals.at(0), GL_STATIC_DRAW));
    }
}

void Model::SetTextureCoords(int const numCoords, GLfloat const *textureCoords)
{

    for (int i = 0; i < numCoords; i++)
    {
        _textureCoords.push_back(textureCoords[i]);
    }
    if (_glInitialized)
    {
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _textureBuffer));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, _textureCoords.size() * sizeof(float), &_textureCoords.at(0), GL_STATIC_DRAW));
    }
}

void Model::SetVertices(int const numVerts, GLfloat const *vertices)
{
    for (int i = 0; i < numVerts; i++)
    {
        _vertices.push_back(vertices[i]);
    }
    if (_glInitialized)
    {
        GL_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertexBuffer));
        GL_CHECK(glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(GLfloat), &_vertices.at(0), GL_STATIC_DRAW));
    }
}

void Model::SetTexture(Image *texture)
{
    _texture = texture;
}

void Model::draw(View *view)
{
    if (_glInitialized)
    {
        // Use our shader
        GL_CHECK(glUseProgram(_programID));

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        glm::mat4 MVP = view->proj * view->view * _model;
        GL_CHECK(glUniformMatrix4fv(_uniformMVP, 1, GL_FALSE, &MVP[0][0]));

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform
        GL_CHECK(glUniformMatrix4fv(_uniformM, 1, GL_FALSE, &view->proj[0][0]));
        GL_CHECK(glUniformMatrix4fv(_uniformV, 1, GL_FALSE, &view->view[0][0]));

        glm::vec3 lightPos = glm::vec3(8, 8, 8);
        GL_CHECK(glUniform3f(_uniformLight, lightPos.x, lightPos.y, lightPos.z));

        // Bind our texture in Texture Unit 0
        GL_CHECK(glActiveTexture(GL_TEXTURE0));
        GL_CHECK(glBindTexture(GL_TEXTURE_2D, _texture->GetTextureGPUHandle()));
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        GL_CHECK(glUniform1i(_uniformSampler1, 0));

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

        // Draw the triangles !
        GL_CHECK(glDrawArrays(GL_TRIANGLES, 0, _vertices.size()));

        GL_CHECK(glDisableVertexAttribArray(0));
        GL_CHECK(glDisableVertexAttribArray(1));
        GL_CHECK(glDisableVertexAttribArray(2));
    }
}

} // namespace OpenDoorGL