//
// Created by barth on 26/09/2022.
//

#include "Triangle.h"

Triangle::Triangle(const char *name, float x, float y, float z) : AbstractMesh(name) {
    setPositionFromFloats(x, y, z);
    std::vector<GLfloat> points = {
            0.0f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
    };
    std::vector<GLint> indices = {0, 1, 2};
    std::vector<GLfloat> normals = {
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f,
            0.0f, 0.0f, -1.0f
    };
    std::vector<GLfloat> colors = {
            1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f
    };
    std::vector<GLfloat> uvs = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f
    };
    setVertexData(&points, &indices, &normals, &uvs, &colors);
}
