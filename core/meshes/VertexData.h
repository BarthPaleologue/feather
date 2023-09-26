//
// Created by barth on 26/09/23.
//

#ifndef FEATHERGL_VERTEXDATA_H
#define FEATHERGL_VERTEXDATA_H

#include <vector>
#include "glad/glad.h"

struct VertexData {
    std::vector<GLfloat> positions;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> uvs;
    std::vector<GLfloat> colors;
    std::vector<GLint> indices;
};

#endif //FEATHERGL_VERTEXDATA_H
