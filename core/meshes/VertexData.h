//
// Created by barth on 26/09/23.
//

#ifndef FEATHERGL_VERTEXDATA_H
#define FEATHERGL_VERTEXDATA_H

#include <vector>
#include <iostream>
#include "glad/glad.h"

struct VertexData {
    std::vector<GLfloat> positions{};
    std::vector<GLfloat> normals{};
    std::vector<GLfloat> uvs{};
    std::vector<GLfloat> colors{};
    std::vector<GLint> indices{};

    void debugPositions() {
        std::cout << "Positions: " << std::endl;
        for (int i = 0; i < positions.size(); i += 3) {
            std::cout << "[" << positions[i] << ", " << positions[i + 1] << ", " << positions[i + 2] << "]"
                      << std::endl;
        }
    }

    void computeNormals() {
        if(normals.empty()) {
            normals.resize(positions.size());
        }
        for (int i = 0; i < indices.size(); i += 3) {
            auto index1 = indices[i];
            auto index2 = indices[i + 1];
            auto index3 = indices[i + 2];
            auto v1 = glm::vec3(positions[index1 * 3], positions[index1 * 3 + 1],
                                positions[index1 * 3 + 2]);
            auto v2 = glm::vec3(positions[index2 * 3], positions[index2 * 3 + 1],
                                positions[index2 * 3 + 2]);
            auto v3 = glm::vec3(positions[index3 * 3], positions[index3 * 3 + 1],
                                positions[index3 * 3 + 2]);
            auto normal = glm::normalize(glm::cross(v2 - v1, v3 - v1));

            normals[index1 * 3] += normal.x;
            normals[index1 * 3 + 1] += normal.y;
            normals[index1 * 3 + 2] += normal.z;
        }
    }
};

#endif //FEATHERGL_VERTEXDATA_H
