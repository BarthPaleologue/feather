//
// Created by barth on 26/09/2022.
//

#include "Triangle.h"

Triangle::Triangle(const char *name, float x, float y, float z) : Mesh(name) {
    setPosition(x, y, z);

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
    setVertexData(vertexData);
}
