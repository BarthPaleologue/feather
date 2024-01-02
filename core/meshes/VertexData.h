//
// Created by barth on 26/09/23.
//

#ifndef FEATHERGL_VERTEXDATA_H
#define FEATHERGL_VERTEXDATA_H

#include <vector>
#include <iostream>
#include <algorithm>
#include "glad/glad.h"
#include "utils.h"

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
            auto normal = glm::cross(v2 - v1, v3 - v1);

            normals[index1 * 3] += normal.x;
            normals[index1 * 3 + 1] += normal.y;
            normals[index1 * 3 + 2] += normal.z;
        }

        // normalize all normals
        for (int i = 0; i < normals.size(); i += 3) {
            auto normal = glm::vec3(normals[i], normals[i + 1], normals[i + 2]);
            normal = glm::normalize(normal);
            normals[i] = normal.x;
            normals[i + 1] = normal.y;
            normals[i + 2] = normal.z;
        }
    }

    /**
     *
     * @return
     * @see See page 5 of original HPBD paper for the description of the algorithm
     */
    VertexData vertexSubset() {
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
            if (coarseMarking[i]) vertexSubset.push_back(i);
        }

        // Triangulate coarse vertices using closest coarse vertex (garbage triangles will be generated so it needs pruning afterward)
        std::vector<GLint> indicesSubset;
        for (int index: indices) {
            if (coarseMarking[index]) {
                indicesSubset.push_back(index);
                continue;
            }

            float minDistance = 1e3;
            int closest = -1;
            glm::vec3 position = glm::vec3(
                    positions[index],
                    positions[index + 1],
                    positions[index + 2]
            );

            for (auto neighbor: neighbors[index]) {
                if (coarseMarking[neighbor]) {
                    glm::vec3 neighborPosition = glm::vec3(
                            positions[neighbor],
                            positions[neighbor + 1],
                            positions[neighbor + 2]
                    );
                    float distance = glm::distance(position, neighborPosition);
                    if (distance >= minDistance) continue;

                    minDistance = distance;
                    closest = neighbor;
                }
            }

            indicesSubset.push_back(closest);
        }

        // prune triangles
        std::vector<GLint> prunedIndicesSubset;
        for (unsigned int i = 0; i < indicesSubset.size(); i += 3) {
            GLint index0 = indicesSubset[i];
            GLint index1 = indicesSubset[i + 1];
            GLint index2 = indicesSubset[i + 2];

            if (index0 == -1 || index1 == -1 || index2 == -1) continue;

            if (index0 == index1 || index1 == index2 || index0 == index2) continue;

            prunedIndicesSubset.push_back(index0);
            prunedIndicesSubset.push_back(index1);
            prunedIndicesSubset.push_back(index2);
        }

        VertexData subset{};
        for (auto vertex: vertexSubset) {
            subset.positions.push_back(positions[vertex * 3]);
            subset.positions.push_back(positions[vertex * 3 + 1]);
            subset.positions.push_back(positions[vertex * 3 + 2]);
            subset.normals.push_back(normals[vertex * 3]);
            subset.normals.push_back(normals[vertex * 3 + 1]);
            subset.normals.push_back(normals[vertex * 3 + 2]);
        }

        for (auto index: prunedIndicesSubset) {
            auto it = std::find(vertexSubset.begin(), vertexSubset.end(), index);
            if (it != vertexSubset.end()) {
                subset.indices.push_back(std::distance(vertexSubset.begin(), it));
            } else {
                throw std::runtime_error("Could not find index of some element in simplify mesh");
            }
        }

        return subset;
    }
};

#endif //FEATHERGL_VERTEXDATA_H
