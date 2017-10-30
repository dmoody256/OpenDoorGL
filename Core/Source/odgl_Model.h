#ifndef ODGL_MODEL
#define ODGL_MODEL

#include "odgl_GeometricObject.h"
#include "odgl_View.h"
#include "odgl_Image.h"

namespace OpenDoorGL{

    class Model : public GeometricObject{

    public:
        Model();
        void SetNormals(int const numNormals, GLfloat const *normals);
        void SetTextureCoords(int const numCoords, GLfloat const *textureCoords);
        void SetVertices(int const numVerts, GLfloat const *vertices);
        void SetTexture(Image* texture);
        void draw(View* view);
    protected:
        Image* _texture;
	    GLuint _uniformV;
        GLuint _uniformM;
        GLuint _uniformSampler1;
        GLuint _uniformLight;
    };

}

#endif