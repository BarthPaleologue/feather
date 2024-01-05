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
     * Creates a subset of the vertex data using an initial triangulation (it could already be a simplification of the original mesh triangulation).
     * It returns the indices of the coarse vertices as well as a substitution table for removed vertices (closest coarse vertex)
     * The returned new triangulation is obtained by using the substitution table on the original triangulation and then removing the garbage triangles.
     * @param originalTriangleIndices
     * @param coarseVertexIndices
     * @param closestCoarseVertexIndices
     * @param prunedTriangleIndicesSubset
     * @see See page 5 of original HPBD paper for the description of the algorithm
     */
    void subset(std::vector<GLint> &originalTriangleIndices, std::vector<GLint> &coarseVertexIndices,
                std::vector<GLint> &closestCoarseVertexIndices, std::vector<GLint> &prunedTriangleIndicesSubset) {
        int k = 2;
        unsigned long nbTotalVertices = positions.size() / 3;

        // All vertices of given triangulation are first marked as coarse
        // In the case of using an already simplified triangulation, not all vertices will be marked as coarse
        std::vector<bool> markedAsCoarse(nbTotalVertices, false);
        for (GLint index: originalTriangleIndices) {
            markedAsCoarse[index] = true;
        }

        // for each vertex, we store the indices of its neighbors
        std::vector<std::vector<GLint>> neighbors(nbTotalVertices, std::vector<GLint>());
        for (unsigned int i = 0; i < originalTriangleIndices.size(); i += 3) {
            GLint index0 = originalTriangleIndices[i];
            GLint index1 = originalTriangleIndices[i + 1];
            GLint index2 = originalTriangleIndices[i + 2];

            neighbors[index0].push_back(index1);
            neighbors[index0].push_back(index2);

            neighbors[index1].push_back(index0);
            neighbors[index1].push_back(index2);

            neighbors[index2].push_back(index0);
            neighbors[index2].push_back(index1);
        }

        // we store the number of coarse neighbors for each vertex
        std::vector<unsigned int> nbCoarseNeighbors(nbTotalVertices, 0);
        for (unsigned int i = 0; i < nbTotalVertices; i++) {
            for (auto neighbor: neighbors[i]) {
                if (markedAsCoarse[neighbor]) nbCoarseNeighbors[i] += 1;
            }
        }

        // Traverse all particles in an arbitrary order
        for (unsigned int i = 0; i < nbTotalVertices; i++) {
//            A particle is marked fine if two conditions are met. First, the number of
//            its coarse neighbors must be greater or equal k.
            if (nbCoarseNeighbors[i] < k) continue;

            // Second, all the neighboring fine vertices must have strictly more than k coarse neighbors.
            bool fineNeighborsCondition = true;
            for (auto neighbor: neighbors[i]) {
                // we skip coarse neighbors
                if (markedAsCoarse[neighbor]) continue;
                // if a fine neighbor has less or equal than k coarse neighbors, it invalidates the condition
                if (nbCoarseNeighbors[neighbor] <= k) {
                    fineNeighborsCondition = false;
                    break;
                }
            }

            if (!fineNeighborsCondition) continue;

            // If the particle is marked as fine, the number of coarse neighbors of all its neighbors is decreased by one.
            markedAsCoarse[i] = false;
            for (auto neighbor: neighbors[i]) {
                nbCoarseNeighbors[neighbor] -= 1;
            }
        }

        for (GLint i = 0; i < nbTotalVertices; i++) {
            if (markedAsCoarse[i]) coarseVertexIndices.push_back(i);
        }

        closestCoarseVertexIndices.resize(nbTotalVertices, -1);
        for (GLint i = 0; i < nbTotalVertices; i++) {
            if (markedAsCoarse[i]) {
                closestCoarseVertexIndices[i] = i;
                continue;
            }

            float minDistance = 1e3;
            int closest = -1;
            glm::vec3 position = glm::vec3(
                    positions[i],
                    positions[i + 1],
                    positions[i + 2]
            );

            for (auto neighbor: neighbors[i]) {
                if (markedAsCoarse[neighbor]) {
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

            closestCoarseVertexIndices[i] = closest;
        }

        // Triangulate coarse vertices using the closest coarse vertex (garbage triangles will be generated, so it needs pruning afterward)
        std::vector<GLint> rawTriangleIndicesSubset;
        for (int index: indices) {
            rawTriangleIndicesSubset.push_back(closestCoarseVertexIndices[index]);
        }

        // prune triangles
        for (unsigned int i = 0; i < rawTriangleIndicesSubset.size(); i += 3) {
            GLint index0 = rawTriangleIndicesSubset[i];
            GLint index1 = rawTriangleIndicesSubset[i + 1];
            GLint index2 = rawTriangleIndicesSubset[i + 2];

            // if any of the vertices could not be substituted to a coarse vertex, skip the triangle
            if (index0 == -1 || index1 == -1 || index2 == -1) continue;

            // if the triangle is degenerate, skip it
            if (index0 == index1 || index1 == index2 || index0 == index2) continue;

            prunedTriangleIndicesSubset.push_back(index0);
            prunedTriangleIndicesSubset.push_back(index1);
            prunedTriangleIndicesSubset.push_back(index2);
        }
    }

    /**
     *
     * @return
     */
    VertexData simplify() {
        std::vector<GLint> coarseVertexIndices;
        std::vector<GLint> closestCoarseVertexIndices;
        std::vector<GLint> prunedIndicesSubset;

        subset(indices, coarseVertexIndices, closestCoarseVertexIndices, prunedIndicesSubset);

        VertexData simplifiedData{};
        for (auto vertex: coarseVertexIndices) {
            simplifiedData.positions.push_back(positions[vertex * 3]);
            simplifiedData.positions.push_back(positions[vertex * 3 + 1]);
            simplifiedData.positions.push_back(positions[vertex * 3 + 2]);
            simplifiedData.normals.push_back(normals[vertex * 3]);
            simplifiedData.normals.push_back(normals[vertex * 3 + 1]);
            simplifiedData.normals.push_back(normals[vertex * 3 + 2]);
        }

        // convert indices to the new range (many vertices have been removed)
        for (auto index: prunedIndicesSubset) {
            auto it = std::find(coarseVertexIndices.begin(), coarseVertexIndices.end(), index);
            if (it != coarseVertexIndices.end()) {
                simplifiedData.indices.push_back(std::distance(coarseVertexIndices.begin(), it));
            } else {
                throw std::runtime_error("Could not find index of some element in simplify mesh");
            }
        }

        return simplifiedData;
    }
};

#endif //FEATHERGL_VERTEXDATA_H
