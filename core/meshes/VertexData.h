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
        if (normals.empty()) {
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

    /**
     *
     * @return
     * @see See page 5 of original HPBD paper for the description of the algorithm
     */
    std::vector<GLint> vertexSubset() {
        int k = 2;
        unsigned long nbVertices = positions.size() / 3;

        // all vertices are first marked as coarse
        std::vector<bool> coarseMarking(nbVertices, true);

        // for each vertex, we store the indices of its neighbors
        std::vector<std::vector<GLint>> neighbors(nbVertices, std::vector<GLint>());
        for (unsigned int i = 0; i < indices.size(); i += 3) {
            GLint index0 = indices[i];
            GLint index1 = indices[i + 1];
            GLint index2 = indices[i + 2];

            neighbors[index0].push_back(index1);
            neighbors[index0].push_back(index2);

            neighbors[index1].push_back(index0);
            neighbors[index1].push_back(index2);

            neighbors[index2].push_back(index0);
            neighbors[index2].push_back(index1);
        }

        // we store the number of coarse neighbors for each vertex (equal to the number of neighbors at startup)
        std::vector<unsigned int> nbCoarseNeighbors(nbVertices, 0);
        for (unsigned int i = 0; i < nbVertices; i++) {
            nbCoarseNeighbors[i] = neighbors[i].size();
        }

        // Traverse all particles in an arbitrary order
        for (unsigned int i = 0; i < nbVertices; i++) {
//            A particle is marked fine if two conditions are met. First, the number of
//            its coarse neighbors must be greater or equal k.
            if (nbCoarseNeighbors[i] < k) continue;

            // Second, all the neighboring fine vertices must have strictly more than k coarse neighbors.
            bool flag = true;
            for (auto neighbor: neighbors[i]) {
                if (coarseMarking[neighbor]) continue;
                if (nbCoarseNeighbors[neighbor] <= k) flag = false;
            }

            if (!flag) continue;

            // If the particle is marked as fine, the number of coarse neighbors of all its neighbors is decreased by one.
            coarseMarking[i] = false;
            for (auto neighbor: neighbors[i]) {
                nbCoarseNeighbors[neighbor] -= 1;
            }
        }

        std::vector<GLint> vertexSubset;
        for (GLint i = 0; i < nbVertices; i++) {
            if (!coarseMarking[i]) vertexSubset.push_back(i);
        }

        return vertexSubset;
    }
};

#endif //FEATHERGL_VERTEXDATA_H
