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

    static std::shared_ptr<Mesh> makeUVCube(const char *name, Scene &scene) {
        // make a cube with 24 vertices instead of 8
        VertexData vertexData = VertexData();
        vertexData.positions = {
                // front
                -1.0f, -1.0f, -1.0f, // 0
                1.0f, -1.0f, -1.0f, // 1
                -1.0f, 1.0f, -1.0f, // 2
                1.0f, 1.0f, -1.0f, // 3

                // right
                1.0f, -1.0f, -1.0f, // 4
                1.0f, -1.0f, 1.0f, // 5
                1.0f, 1.0f, -1.0f, // 6
                1.0f, 1.0f, 1.0f, // 7

                // back
                1.0f, -1.0f, 1.0f, // 8
                -1.0f, -1.0f, 1.0f, // 9
                1.0f, 1.0f, 1.0f, // 10
                -1.0f, 1.0f, 1.0f, // 11

                // left
                -1.0f, -1.0f, 1.0f, // 12
                -1.0f, -1.0f, -1.0f, // 13
                -1.0f, 1.0f, 1.0f, // 14
                -1.0f, 1.0f, -1.0f, // 15

                // top
                -1.0f, 1.0f, -1.0f, // 16
                1.0f, 1.0f, -1.0f, // 17
                -1.0f, 1.0f, 1.0f, // 18
                1.0f, 1.0f, 1.0f, // 19

                // bottom
                -1.0f, -1.0f, 1.0f, // 20
                1.0f, -1.0f, 1.0f, // 21
                -1.0f, -1.0f, -1.0f, // 22
                1.0f, -1.0f, -1.0f, // 23
        };

        vertexData.colors = {
                // front
                1.0f, 1.0f, 1.0f, // 0
                1.0f, 0.0f, 1.0f, // 1
                1.0f, 1.0f, 0.0f, // 2
                0.0f, 1.0f, 1.0f, // 3

                // right
                1.0f, 1.0f, 0.0f, // 4
                1.0f, 0.0f, 0.0f, // 5
                0.0f, 1.0f, 0.0f, // 6
                0.0f, 0.0f, 0.0f, // 7

                // back
                0.0f, 1.0f, 0.0f, // 8
                0.0f, 0.0f, 0.0f, // 9
                0.0f, 1.0f, 1.0f, // 10
                0.0f, 0.0f, 1.0f, // 11

                // left
                0.0f, 1.0f, 1.0f, // 12
                0.0f, 0.0f, 1.0f, // 13
                1.0f, 1.0f, 1.0f, // 14
                1.0f, 0.0f, 1.0f, // 15

                // top
                1.0f, 1.0f, 1.0f, // 16
                1.0f, 0.0f, 1.0f, // 17
                0.0f, 1.0f, 1.0f, // 18
                0.0f, 0.0f, 1.0f, // 19

                // bottom
                1.0f, 0.0f, 0.0f, // 20
                0.0f, 0.0f, 0.0f, // 21
                1.0f, 0.0f, 1.0f, // 22
                0.0f, 0.0f, 1.0f, // 23
        };


        vertexData.normals = {
                // front
                0.0f, 0.0f, -1.0f, // 0
                0.0f, 0.0f, -1.0f, // 1
                0.0f, 0.0f, -1.0f, // 2
                0.0f, 0.0f, -1.0f, // 3

                // right
                1.0f, 0.0f, 0.0f, // 4
                1.0f, 0.0f, 0.0f, // 5
                1.0f, 0.0f, 0.0f, // 6
                1.0f, 0.0f, 0.0f, // 7

                // back
                0.0f, 0.0f, 1.0f, // 8
                0.0f, 0.0f, 1.0f, // 9
                0.0f, 0.0f, 1.0f, // 10
                0.0f, 0.0f, 1.0f, // 11

                // left
                -1.0f, 0.0f, 0.0f, // 12
                -1.0f, 0.0f, 0.0f, // 13
                -1.0f, 0.0f, 0.0f, // 14
                -1.0f, 0.0f, 0.0f, // 15

                // top
                0.0f, 1.0f, 0.0f, // 16
                0.0f, 1.0f, 0.0f, // 17
                0.0f, 1.0f, 0.0f, // 18
                0.0f, 1.0f, 0.0f, // 19

                // bottom
                0.0f, -1.0f, 0.0f, // 20
                0.0f, -1.0f, 0.0f, // 21
                0.0f, -1.0f, 0.0f, // 22
                0.0f, -1.0f, 0.0f, // 23
        };

        vertexData.indices = {
                // front
                0, 2, 3,
                0, 3, 1,

                // right
                4, 6, 7,
                4, 7, 5,

                // back
                8, 10, 11,
                8, 11, 9,

                // left
                12, 14, 15,
                12, 15, 13,

                // top
                16, 18, 19,
                16, 19, 17,

                // bottom
                20, 22, 23,
                20, 23, 21,
        };

        vertexData.uvs = {
                // front
                0.0f, 1.0f, // 0
                1.0f, 1.0f, // 1
                1.0f, 0.0f, // 2
                0.0f, 0.0f, // 3

                // right
                0.0f, 0.0f, // 4
                1.0f, 0.0f, // 5
                1.0f, 1.0f, // 6
                0.0f, 1.0f, // 7

                // back
                0.0f, 1.0f, // 8
                1.0f, 1.0f, // 9
                1.0f, 0.0f, // 10
                0.0f, 0.0f, // 11

                // left
                0.0f, 0.0f, // 12
                1.0f, 0.0f, // 13
                1.0f, 1.0f, // 14
                0.0f, 1.0f, // 15

                // top
                0.0f, 1.0f, // 16
                1.0f, 1.0f, // 17
                1.0f, 0.0f, // 18
                0.0f, 0.0f, // 19

                // bottom
                0.0f, 0.0f, // 20
                1.0f, 0.0f, // 21
                1.0f, 1.0f, // 22
                0.0f, 1.0f, // 23
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

    static std::shared_ptr<Mesh> makeUvSphere(const char *name, Scene &scene, int resolution) {
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

    static std::shared_ptr<Mesh> makeIcoSphere(const char *name, Scene &scene, unsigned int nbSubdivisions) {
        const float X = .525731112119133606f;
        const float Z = .850650808352039932f;
        const float N = 0.f;

        VertexData vertexData = VertexData();

        vertexData.positions = {
                -X, N, Z, X, N, Z, -X, N, -Z, X, N, -Z,
                N, Z, X, N, Z, -X, N, -Z, X, N, -Z, -X,
                Z, X, N, -Z, X, N, Z, -X, N, -Z, -X, N
        };

        vertexData.indices = {
                        0,  4,  1,
                        0,  9,  4,
                        9,  5,  4,
                        4,  5,  8,
                        4,  8,  1,
                        8,  10, 1,
                        8,  3,  10,
                        5,  3,  8,
                        5,  2,  3,
                        2,  7,  3,
                        7,  10, 3,
                        7,  6,  10,
                        7,  11, 6,
                        11, 0,  6,
                        0,  1,  6,
                        6,  1,  10,
                        9,  0,  11,
                        9,  11, 2,
                        9,  2,  5,
                        7,  2,  11
                };

        vertexData.computeNormals();

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

    static std::shared_ptr<Mesh> makeLine(const char *name, Scene &scene, glm::vec3 p1, glm::vec3 p2) {
        VertexData vertexData = VertexData();
        vertexData.positions = {
                p1.x, p1.y, p1.z,
                p2.x, p2.y, p2.z
        };
        vertexData.indices = {
                0, 1
        };
        vertexData.normals = {
                0.0f, 0.0f, 0.0f,
                0.0f, 0.0f, -1.0f
        };
        vertexData.uvs = {
                0.0f, 0.0f,
                1.0f, 1.0f
        };

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

    static std::shared_ptr<Mesh> Simplify(const char *name, std::shared_ptr<Mesh> original, Scene &scene) {
        auto simplifiedData = original->vertexData().vertexSubset();
        auto simplifiedMesh = Mesh::FromVertexData(name, simplifiedData);
        scene.addMesh(simplifiedMesh);

        return simplifiedMesh;
    }

    static std::shared_ptr<Mesh> FromObjFile(std::string filePath, Scene &scene) {
        std::ifstream file(filePath);
        std::string line;
        std::vector<GLfloat> positions;
        std::vector<GLfloat> normals;
        std::vector<GLfloat> uvs;
        std::vector<GLint> indices;
        std::vector<GLfloat> colors;

        while (std::getline(file, line)) {
            std::istringstream iss(line);
            std::string type;
            iss >> type;
            if (type == "v") {
                glm::vec3 position;
                iss >> position.x >> position.y >> position.z;
                positions.push_back(position.x);
                positions.push_back(position.y);
                positions.push_back(position.z);
            } else if (type == "vn") {
                glm::vec3 normal;
                iss >> normal.x >> normal.y >> normal.z;
                normals.push_back(normal.x);
                normals.push_back(normal.y);
                normals.push_back(normal.z);
            } else if (type == "vt") {
                glm::vec2 uv;
                iss >> uv.x >> uv.y;
                uvs.push_back(uv.x);
                uvs.push_back(uv.y);
            } else if (type == "f") {
                std::string vertex1, vertex2, vertex3;
                iss >> vertex1 >> vertex2 >> vertex3;
                indices.push_back(std::stoi(vertex1) - 1);
                indices.push_back(std::stoi(vertex2) - 1);
                indices.push_back(std::stoi(vertex3) - 1);
            }
        }

        for (int i = 0; i < positions.size(); i++) colors.push_back(1.0f);

        VertexData vertexData;
        vertexData.positions = positions;
        vertexData.normals = normals;
        vertexData.uvs = uvs;
        vertexData.indices = indices;
        vertexData.colors = colors;

        auto mesh = Mesh::FromVertexData(filePath.c_str(), vertexData);
        scene.addMesh(mesh);
        return mesh;
    }
};

#endif //FEATHERGL_MESHBUILDER_H
