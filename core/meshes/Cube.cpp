//
// Created by barth on 27/09/2022.
//

#include "Cube.h"

Cube::Cube(const char *name, float x, float y, float z) : AbstractMesh(name) {
    setPosition(x, y, z);
    std::vector<float> vertices = {
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
    };
    std::vector<float> colors = {
            1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f
    };
    std::vector<int> indices = {
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
    std::vector<float> normals = {
            -0.577350, -0.577350, -0.577350,
            0.816497, -0.408248, -0.408248,
            -0.408248, 0.816497, -0.408248,
            0.408248, 0.408248, -0.816497,
            -0.408248, -0.408248, 0.816497,
            0.408248, -0.816497, 0.408248,
            -0.816497, 0.408248, 0.408248,
            0.577350, 0.577350, 0.577350,
    };
    std::vector<float> uvs = {
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f,

            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f,
    };
    setVertexData(&vertices, &indices, &normals, &uvs, &colors);
}
