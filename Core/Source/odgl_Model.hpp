#ifndef ODGL_MODEL
#define ODGL_MODEL

#include "odgl_Include.hpp"
#include "odgl_GeometricObject.hpp"
#include "odgl_View.hpp"
#include "odgl_Image.hpp"

namespace OpenDoorGL
{

class ODGL_DLLEXPORT Model : public GeometricObject
{

public:
  ~Model();
  Model(bool init_gl = true);
  void SetNormals(int const numNormals, GLfloat const *normals);
  void SetTextureCoords(int const numCoords, GLfloat const *textureCoords);
  void SetVertices(int const numVerts, GLfloat const *vertices);
  void SetTexture(Image *texture);
  void draw(View *view);

protected:
  Image *_texture;
  GLuint _uniformV;
  GLuint _uniformM;
  GLuint _uniformSampler1;
  GLuint _uniformLight;

private:
  void InitGL();
};

} // namespace OpenDoorGL

#endif