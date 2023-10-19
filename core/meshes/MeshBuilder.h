//
// Created by barth on 26/09/23.
//

#ifndef FEATHERGL_MESHBUILDER_H
#define FEATHERGL_MESHBUILDER_H

#include <iostream>
#include "Mesh.h"
#include "Scene.h"

class MeshBuilder {
public:
    static std::shared_ptr<Mesh> makeCube(const char *name, Scene &scene) {
        VertexData vertexData = VertexData();
        vertexData.positions = {
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                1.0f, 1.0f, -1.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
        };
        vertexData.colors = {
                1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 0.0f,
                0.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 0.0f,
                0.0f, 0.0f, 1.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f
        };
        vertexData.indices = {
                0, 2, 3,
                0, 3, 1,
                0, 1, 5,
                0, 5, 4,
                0, 4, 6,
                0, 6, 2,
                1, 3, 7,
                1, 7, 5,
                2, 6, 7,
                2, 7, 3,
                4, 5, 7,
                4, 7, 6
        };
        vertexData.normals = {
                -0.577350, -0.577350, -0.577350,
                0.816497, -0.408248, -0.408248,
                -0.408248, 0.816497, -0.408248,
                0.408248, 0.408248, -0.816497,
                -0.408248, -0.408248, 0.816497,
                0.408248, -0.816497, 0.408248,
                -0.816497, 0.408248, 0.408248,
                0.577350, 0.577350, 0.577350,
        };
        vertexData.uvs = {
                0.0f, 1.0f,
                1.0f, 1.0f,
                1.0f, 0.0f,
                0.0f, 0.0f,

                0.0f, 0.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f,
        };

        auto mesh = Mesh::FromVertexData(name, vertexData);
        scene.addMesh(mesh);
        return mesh;
    }

    static std::shared_ptr<Mesh> makeTriangle(const char *name, Scene &scene) {
        VertexData vertexData = VertexData();
        vertexData.positions = {
                0.0f, 0.5f, 0.0f,
                0.5f, -0.5f, 0.0f,
                -0.5f, -0.5f, 0.0f
        };
        vertexData.indices = {0, 1, 2};
        vertexData.normals = {
                0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, -1.0f
        };
        vertexData.normals = {
                1.0f, 0.0f, 0.0f,
                0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 1.0f
        };
        vertexData.uvs = {
                0.0f, 0.0f,
                1.0f, 0.0f,
                0.0f, 1.0f
        };

        auto mesh = Mesh::FromVertexData(name, vertexData);
        scene.addMesh(mesh);
        return mesh;
    }

    static std::shared_ptr<Mesh> makeSphere(const char *name, Scene &scene, int resolution) {
        std::vector<GLfloat> positions;
        std::vector<GLfloat> normals;
        std::vector<GLfloat> uvs;
        std::vector<GLint> indices;
        std::vector<GLfloat> colors;

        const float PI = std::acos(-1.0f);

        float x, y, z, xy;                              // vertex position
        float nx, ny, nz, lengthInv = 1.0f;    // normal
        float s, t;                                     // texCoord

        float sectorStep = 2 * PI / (float) resolution;
        float stackStep = PI / (float) resolution;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= resolution; ++i) {
            stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
            xy = cosf(stackAngle);             // r * cos(u)
            z = sinf(stackAngle);              // r * sin(u)

            // add (sectorCount+1) positions per stack
            // the first and last positions have same position and normal, but different tex coords
            for (int j = 0; j <= resolution; ++j) {
                sectorAngle = j * sectorStep;           // starting from 0 to 2pi

                // vertex position
                x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
                y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
                positions.push_back(x);
                positions.push_back(z);
                positions.push_back(y);

                // normalized vertex normal
                nx = x * lengthInv;
                ny = z * lengthInv;
                nz = y * lengthInv;
                normals.push_back(nx);
                normals.push_back(ny);
                normals.push_back(nz);

                // vertex tex coord between [0, 1]
                s = (float) j / (float) resolution;
                t = (float) i / (float) resolution;
                uvs.push_back(1.0f - s);
                uvs.push_back(t);
            }
        }

        // indices
        //  k1--k1+1
        //  |  / |
        //  | /  |
        //  k2--k2+1
        unsigned int k1, k2;
        for (int i = 0; i < resolution; ++i) {
            k1 = i * (resolution + 1);     // beginning of current stack
            k2 = k1 + resolution + 1;      // beginning of next stack

            for (int j = 0; j < resolution; ++j, ++k1, ++k2) {
                // 2 triangles per sector excluding 1st and last stacks
                if (i != 0) {
                    indices.push_back(k1);
                    //indices.push_back(k2);
                    indices.push_back(k1 + 1);   // k1---k2---k1+1
                    indices.push_back(k2);
                }

                if (i != (resolution - 1)) {
                    indices.push_back(k1 + 1);
                    //indices.push_back(k2);
                    indices.push_back(k2 + 1); // k1+1---k2---k2+1
                    indices.push_back(k2);
                }
            }
        }

        for (int i = 0; i < positions.size(); i++) colors.push_back(1.0f);

        VertexData vertexData;
        vertexData.positions = positions;
        vertexData.normals = normals;
        vertexData.uvs = uvs;
        vertexData.indices = indices;
        vertexData.colors = colors;

        auto mesh = Mesh::FromVertexData(name, vertexData);
        scene.addMesh(mesh);
        return mesh;
    }

    static std::shared_ptr<Mesh> makePlane(const char *name, Scene &scene, unsigned int nbSubdivisions) {
        VertexData vertexData = VertexData();
        vertexData.positions = std::vector<GLfloat>(nbSubdivisions * nbSubdivisions * 3);
        vertexData.indices = std::vector<GLint>((nbSubdivisions - 1) * (nbSubdivisions - 1) * 3 * 2);
        vertexData.uvs = std::vector<GLfloat>(nbSubdivisions * nbSubdivisions * 2);
        vertexData.normals = std::vector<GLfloat>(nbSubdivisions * nbSubdivisions * 3);

        float size = 1.0f;

        for (int x = 0; x < nbSubdivisions; x++) {
            float xPosition = ((float) x / (float) (nbSubdivisions - 1)) * size - size / 2.0f;
            float xUV = (float) x / (float) (nbSubdivisions - 1);

            for (int z = 0; z < nbSubdivisions; z++) {
                float zPosition = ((float) z / (float) (nbSubdivisions - 1)) * size - size / 2.0f;
                float zUV = (float) z / (float) (nbSubdivisions - 1);

                int positionIndex = 3 * (x * nbSubdivisions + z);
                vertexData.positions[positionIndex] = xPosition;
                vertexData.positions[positionIndex + 1] = 0;
                vertexData.positions[positionIndex + 2] = zPosition;

                int normalIndex = 3 * (x * nbSubdivisions + z);
                vertexData.normals[normalIndex] = 0;
                vertexData.normals[normalIndex + 1] = 1;
                vertexData.normals[normalIndex + 2] = 0;

                int uvIndex = 2 * (x * nbSubdivisions + z);
                vertexData.uvs[uvIndex] = xUV;
                vertexData.uvs[uvIndex + 1] = zUV;

                if (x == nbSubdivisions - 1 || z == nbSubdivisions - 1) continue;

                int index = 6 * (x * (nbSubdivisions - 1) + z);
                vertexData.indices[index] = (x + 1) * nbSubdivisions + z;
                vertexData.indices[index + 1] = x * nbSubdivisions + z;
                vertexData.indices[index + 2] = x * nbSubdivisions + z + 1;

                vertexData.indices[index + 3] = (x + 1) * nbSubdivisions + z;
                vertexData.indices[index + 4] = x * nbSubdivisions + z + 1;
                vertexData.indices[index + 5] = (x + 1) * nbSubdivisions + z + 1;
            }
        }

        auto mesh = Mesh::FromVertexData(name, vertexData);
        scene.addMesh(mesh);
        return mesh;
    }

    static std::shared_ptr<Mesh> makeScreenQuad(const char *name) {
        VertexData vertexData = VertexData();
        vertexData.positions = {
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                -1.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 0.0f
        };
        vertexData.indices = {
                0, 1, 2,
                1, 3, 2
        };
        vertexData.uvs = {
                0.0f, 0.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f
        };
        vertexData.normals = {
                0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, -1.0f,
                0.0f, 0.0f, -1.0f
        };

        return Mesh::FromVertexData(name, vertexData);
    }
};

#endif //FEATHERGL_MESHBUILDER_H
