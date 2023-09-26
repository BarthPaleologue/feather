//
// Created by barth on 27/09/2022.
//

#include "Cube.h"

Cube::Cube(const char *name, float x, float y, float z) : Mesh(name) {
    setPosition(x, y, z);

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

    setVertexData(vertexData);
}
