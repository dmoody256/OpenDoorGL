#ifndef ODGL_GEOMETRIC_OBJECT_H
#define ODGL_GEOMETRIC_OBJECT_H

#include <GLEW/glew.h>

#include <vector>
#include <string>

#include "odgl_Include.hpp"
#include "odgl_RenderObject.hpp"

namespace OpenDoorGL
{

class ODGL_DLLEXPORT GeometricObject : public RenderObject
{

public:
  GeometricObject();
  explicit GeometricObject(bool init_gl);
  ~GeometricObject();

  void draw(View view);

  void setShadersFromString(const char *vert_shader, const char *frag_shader);

  virtual Vector getMaxBounds();
  virtual Vector getMinBounds();

  virtual float getMinX();
  virtual float getMinY();
  virtual float getMinZ();
  virtual float getMaxX();
  virtual float getMaxY();
  virtual float getMaxZ();

protected:
  GLuint _vertexBuffer;
  GLuint _colorBuffer;
  GLuint _textureBuffer;
  GLuint _normalBuffer;

  std::vector<GLfloat> _vertices;
  std::vector<GLfloat> _vertColors;
  std::vector<GLfloat> _textureCoords;
  std::vector<GLfloat> _normals;

  GLuint _uniformV;
  GLuint _uniformM;
  GLuint _uniformSampler1;
  GLuint _uniformLight;

  GLuint _programID;
  GLuint _uniformMVP;

  std::string _vertShader;
  std::string _fragShader;

private:
  void InitGL();
};
} // namespace OpenDoorGL

#endif