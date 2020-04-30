#include "odgl_ModelLoader.hpp"
#include "odgl_Include.hpp"

#include <vector>
#include <iostream>
#include <cstring>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace OpenDoorGL
{

    Model *ModelLoader::LoadOBJFile(const char *path, bool init_gl)
    {
        ODGL_DEBUG_PRINT(std::cout << "Loading OBJ file " << path << "..." << std::endl);

        std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
        std::vector<glm::vec3> temp_vertices;
        std::vector<glm::vec2> temp_uvs;
        std::vector<glm::vec3> temp_normals;

        FILE *file = fopen(path, "r");
        if (file == NULL)
        {
            printf("ERROR: Failed to open OBJ file.\n");
            getchar();
            return nullptr;
        }

        while (1)
        {

            char lineHeader[128];
            // read the first word of the line
            int res = fscanf(file, "%127s", lineHeader);
            if (res == EOF)
                break; // EOF = End Of File. Quit the loop.

            // else : parse lineHeader

            if (strcmp(lineHeader, "v") == 0)
            {
                glm::vec3 vertex;
                if (fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z) != 3)
                {
                    printf("ERROR: read the wrong number of vertives.\n");
                }
                temp_vertices.push_back(vertex);
            }
            else if (strcmp(lineHeader, "vt") == 0)
            {
                glm::vec2 uv;
                if (fscanf(file, "%f %f\n", &uv.x, &uv.y) != 2)
                {
                    printf("ERROR: read the wrong number of texture coords.\n");
                }
                uv.y = -uv.y; // Invert V coordinate since we will only use DDS texture, which are inverted. Remove if you want to use TGA or BMP loaders.
                temp_uvs.push_back(uv);
            }
            else if (strcmp(lineHeader, "vn") == 0)
            {
                glm::vec3 normal;
                if (fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z) != 3)
                {
                    printf("ERROR: read the wrong number of normals.\n");
                }
                temp_normals.push_back(normal);
            }
            else if (strcmp(lineHeader, "f") == 0)
            {
                std::string vertex1, vertex2, vertex3;
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(file, "%10u/%10u/%10u %10u/%10u/%10u %10u/%10u/%10u\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
                if (matches != 9)
                {
                    printf("File can't be read by our simple parser :-( Try exporting with other options\n");
                    fclose(file);
                    return nullptr;
                }
                vertexIndices.push_back(vertexIndex[0]);
                vertexIndices.push_back(vertexIndex[1]);
                vertexIndices.push_back(vertexIndex[2]);
                uvIndices.push_back(uvIndex[0]);
                uvIndices.push_back(uvIndex[1]);
                uvIndices.push_back(uvIndex[2]);
                normalIndices.push_back(normalIndex[0]);
                normalIndices.push_back(normalIndex[1]);
                normalIndices.push_back(normalIndex[2]);
            }
            else
            {
                // Probably a comment, eat up the rest of the line
                char stupidBuffer[1000];
                if (fgets(stupidBuffer, 1000, file) == NULL)
                {
                    printf("WARN: fgets returned NULL while eating up the rest of the line\n");
                }
            }
        }

        Model *geoObject = new Model(init_gl);
        std::vector<GLfloat> out_vertices;
        std::vector<GLfloat> out_uvs;
        std::vector<GLfloat> out_normals;

        // For each vertex of each triangle
        for (unsigned int i = 0; i < vertexIndices.size(); i++)
        {

            // Get the indices of its attributes
            unsigned int vertexIndex = vertexIndices[i];
            unsigned int uvIndex = uvIndices[i];
            unsigned int normalIndex = normalIndices[i];

            // Get the attributes thanks to the index
            glm::vec3 vertex = temp_vertices[vertexIndex - 1];
            glm::vec2 uv = temp_uvs[uvIndex - 1];
            glm::vec3 normal = temp_normals[normalIndex - 1];

            // Put the attributes in buffers
            out_vertices.push_back(vertex.x);
            out_vertices.push_back(vertex.y);
            out_vertices.push_back(vertex.z);

            out_uvs.push_back(uv.x);
            out_uvs.push_back(uv.y);

            out_normals.push_back(normal.x);
            out_normals.push_back(normal.y);
            out_normals.push_back(normal.z);
        }
        geoObject->SetTextureCoords(out_uvs.size(), &out_uvs.at(0));
        geoObject->SetVertices(out_vertices.size(), &out_vertices.at(0));
        geoObject->SetNormals(out_normals.size(), &out_normals.at(0));

        fclose(file);
        return geoObject;
    }
} // namespace OpenDoorGL
