//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_UTILS_H
#define FEATHERGL_UTILS_H

#include <string>
#include <vector>
#include <map>
#include <glm/vec3.hpp>
#include <iostream>
#include <glm/gtc/random.hpp>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include "glad/glad.h"
#include "Core"
#include "PickResult.h"

/**
 * Load file given by filename to the stringBuffer
 * @param filename
 * @param stringBuffer
 */
void loadFileToBuffer(const char *filename, std::string &stringBuffer);

GLuint loadTextureFromFileToGPU(const char *filename);

void writeTextureToPPM(GLuint textureHandle, const char *filename);

void writeDepthTextureToPPM(GLuint textureHandle, const char *filename);

std::string toString(glm::vec3 vec);

class Utils {
public:
    template<typename T>
    static void DebugVector(const std::vector<T> &vec, const std::string &label) {
        if (!label.empty()) {
            std::cout << label << ": ";
        }

        std::cout << "[";
        for (size_t i = 0; i < vec.size(); ++i) {
            std::cout << vec[i];
            if (i < vec.size() - 1) {
                std::cout << ", ";
            }
        }
        std::cout << "]" << std::endl;
    }

    static glm::vec3 RandomDirection() {
        float theta = glm::linearRand(0.f, 2.f * glm::pi<float>());
        float phi = glm::linearRand(0.f, glm::pi<float>());
        return {glm::sin(phi) * glm::cos(theta), glm::cos(phi), glm::sin(phi) * glm::sin(theta)};
    }

    static void Subdivide(std::vector<float>& positions, std::vector<int>& indices) {
        std::vector<int> newIndices;

        for(int i = 0; i < indices.size(); i += 3) {
            int index1 = indices[i];
            int index2 = indices[i + 1];
            int index3 = indices[i + 2];

            glm::vec3 v1 = glm::vec3(positions[index1 * 3], positions[index1 * 3 + 1], positions[index1 * 3 + 2]);
            glm::vec3 v2 = glm::vec3(positions[index2 * 3], positions[index2 * 3 + 1], positions[index2 * 3 + 2]);
            glm::vec3 v3 = glm::vec3(positions[index3 * 3], positions[index3 * 3 + 1], positions[index3 * 3 + 2]);

            glm::vec3 v12 = (v1 + v2) / 2.f;
            glm::vec3 v23 = (v2 + v3) / 2.f;
            glm::vec3 v31 = (v3 + v1) / 2.f;

            int index12 = positions.size() / 3;
            int index23 = positions.size() / 3 + 1;
            int index31 = positions.size() / 3 + 2;

            positions.push_back(v12.x);
            positions.push_back(v12.y);
            positions.push_back(v12.z);

            positions.push_back(v23.x);
            positions.push_back(v23.y);
            positions.push_back(v23.z);

            positions.push_back(v31.x);
            positions.push_back(v31.y);
            positions.push_back(v31.z);

            newIndices.push_back(index1);
            newIndices.push_back(index12);
            newIndices.push_back(index31);

            newIndices.push_back(index2);
            newIndices.push_back(index23);
            newIndices.push_back(index12);

            newIndices.push_back(index3);
            newIndices.push_back(index31);
            newIndices.push_back(index23);

            newIndices.push_back(index12);
            newIndices.push_back(index23);
            newIndices.push_back(index31);
        }

        indices = newIndices;
    }

    static void MergeVertices(std::vector<float>& positions, std::vector<int>& indices) {
        std::vector<float> newPositions;
        std::vector<int> newIndices;

        // remove vertices that share the same position
        for(int i = 0; i < positions.size(); i += 3) {
            glm::vec3 vertex = glm::vec3(positions[i], positions[i + 1], positions[i + 2]);
            bool isDuplicated = false;
            int originalIndex = -1;
            for(int j = 0; j < positions.size() - 3; j+= 3) {
                glm::vec3 otherVertex = glm::vec3(positions[j], positions[j + 1], positions[j + 2]);
                if(glm::distance(vertex, otherVertex) < 0.001f) {
                    isDuplicated = true;
                    originalIndex = j;
                    break;
                }
            }
            if(!isDuplicated) continue;

            if(i / 3 == originalIndex / 3) {
                continue;
            }

            for(int j = 0; j < indices.size(); j++) {
                if(indices[j] == i / 3) {
                    indices[j] = originalIndex / 3;
                }
            }

            // decrease all indices greater than i / 3 by 1
            for(int j = 0; j < indices.size(); j++) {
                if(indices[j] > i / 3) {
                    indices[j]--;
                }
            }

            // remove vertex
            positions.erase(positions.begin() + i, positions.begin() + i + 3);
            i -= 3;
        }
    }

    static bool rayTriangleIntersection(glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::vec3 t0, glm::vec3 t1, glm::vec3 t2, glm::vec3 &result) {
        glm::vec3 edge1 = t1 - t0;
        glm::vec3 edge2 = t2 - t0;
        glm::vec3 h = glm::cross(rayDirection, edge2);
        float a = glm::dot(edge1, h);

        if (a > -0.00001f && a < 0.00001f) {
            return false;
        }

        float f = 1.0f / a;
        glm::vec3 s = rayOrigin - t0;
        float u = f * glm::dot(s, h);

        if (u < 0.0f || u > 1.0f) {
            return false;
        }

        glm::vec3 q = glm::cross(s, edge1);
        float v = f * glm::dot(rayDirection, q);

        if (v < 0.0f || u + v > 1.0f) {
            return false;
        }

        float t = f * glm::dot(edge2, q);

        if (t > 0.00001f) {
            result = rayOrigin + rayDirection * t;
            return true;
        }

        return false;
    }

    static Eigen::MatrixXf crossProdMat(Eigen::VectorXf _p) {
        Eigen::MatrixXf crossMat(3,3);
        crossMat << 0, -_p.z(), _p.y(),
                _p.z(), 0, -_p.x(),
                -_p.y(), _p.x(), 0;
        return crossMat;
    }

    static Eigen::MatrixXf crossProdGrad_p1(Eigen::Vector3f _p1, Eigen::Vector3f _p2) {
        Eigen::MatrixXf _pTilde = crossProdMat(_p2);
        Eigen::Vector3f _n = _p1.cross(_p2);
        double crossNorm = _n.norm();
        _n = _n.normalized();
        return (- _pTilde + _n*(_n.cross(_p2).transpose()))/crossNorm;
    }

    static Eigen::MatrixXf crossProdGrad_p2(Eigen::Vector3f _p1, Eigen::Vector3f _p2) {
        Eigen::MatrixXf _pTilde = crossProdMat(_p1);
        Eigen::Vector3f _n = _p1.cross(_p2);
        double crossNorm = _n.norm();
        _n = _n.normalized();
        return -(- _pTilde + _n*(_n.cross(_p1).transpose()))/crossNorm;
    }

    /**
     * Compute the Euler-Poincare characteristic of a given triangulation in 3D
     * @param indices the indices of the triangulation
     * @return the Euler-Poincare characteristic. Returns 2 if it is a convex polyhedron
     */
    static int EulerPoincareCharacteristic(std::vector<int> &indices) {
        int numberOfFaces = indices.size() / 3;

        // compute number of vertices
        std::map<int, int> vertexDegree;
        for(int i = 0; i < indices.size(); i++) {
            vertexDegree[indices[i]]++;
        }
        int numberOfVertices = vertexDegree.size();

        // compute number of edges
        std::map<std::pair<int, int>, int> edgeCount;
        for(int i = 0; i < indices.size(); i += 3) {
            int v1 = indices[i];
            int v2 = indices[i + 1];
            int v3 = indices[i + 2];

            edgeCount[std::make_pair(std::min(v1, v2), std::max(v1, v2))]++;
            edgeCount[std::make_pair(std::min(v2, v3), std::max(v2, v3))]++;
            edgeCount[std::make_pair(std::min(v1, v3), std::max(v1, v3))]++;
        }
        int numberOfEdges = edgeCount.size();

        return numberOfVertices - numberOfEdges + numberOfFaces;
    }

    /**
     * Check if a given triangulation is closed by checking if each edge is shared by exactly 2 faces
     * @param indices the indices of the triangulation
     * @return true if the triangulation is closed, false otherwise
     */
    static bool IsTriangulationClosed(std::vector<int> &indices) {
        std::map<std::pair<int, int>, int> edgeCount;
        for(int i = 0; i < indices.size(); i += 3) {
            int v1 = indices[i];
            int v2 = indices[i + 1];
            int v3 = indices[i + 2];

            edgeCount[std::make_pair(std::min(v1, v2), std::max(v1, v2))]++;
            edgeCount[std::make_pair(std::min(v2, v3), std::max(v2, v3))]++;
            edgeCount[std::make_pair(std::min(v1, v3), std::max(v1, v3))]++;
        }

        return std::all_of(edgeCount.begin(), edgeCount.end(), [](std::pair<std::pair<int, int>, int> edge) {
            return edge.second == 2;
        });
    }

    static bool isMergedTriangulationClosed(std::vector<int> &indices, std::vector<GLfloat> &positions) {
        // make a copy of the indices and the positions
        std::vector<int> indicesCopy = std::vector<int>(indices);
        std::vector<GLfloat> positionsCopy = std::vector<GLfloat>(positions);

        // merge vertices that share the same position
        MergeVertices(positionsCopy, indicesCopy);

        // now check if the triangulation is closed
        return IsTriangulationClosed(indicesCopy);
    }

    /**
     * Remove an edge in all triangles inside the triangulation
     * @param indices the indices of the triangulation
     * @param edge0 the first vertex of the edge
     * @param edge1 the second vertex of the edge
     */
    static void RemoveEdge(std::vector<int> &indices, int edge0, int edge1) {
        for(int i = 0; i < indices.size(); i += 3) {
            int v1 = indices[i];
            int v2 = indices[i + 1];
            int v3 = indices[i + 2];

            if((v1 == edge0 && v2 == edge1) || (v1 == edge1 && v2 == edge0)) {
                indices.erase(indices.begin() + i, indices.begin() + i + 3);
                i -= 3;
            } else if((v2 == edge0 && v3 == edge1) || (v2 == edge1 && v3 == edge0)) {
                indices.erase(indices.begin() + i, indices.begin() + i + 3);
                i -= 3;
            } else if((v1 == edge0 && v3 == edge1) || (v1 == edge1 && v3 == edge0)) {
                indices.erase(indices.begin() + i, indices.begin() + i + 3);
                i -= 3;
            }
        }
    }

    static PickResult PickWithRay(std::vector<int> &indices, std::vector<GLfloat> &positions, glm::vec3 rayOrigin, glm::vec3 rayDirection, glm::mat4 worldMatrix) {
        std::vector<PickResult> results{};

        for(int i = 0; i < indices.size(); i += 3) {
            int index1 = indices[i];
            int index2 = indices[i + 1];
            int index3 = indices[i + 2];

            glm::vec3 v1 = glm::vec3(positions[index1 * 3], positions[index1 * 3 + 1], positions[index1 * 3 + 2]);
            glm::vec3 v2 = glm::vec3(positions[index2 * 3], positions[index2 * 3 + 1], positions[index2 * 3 + 2]);
            glm::vec3 v3 = glm::vec3(positions[index3 * 3], positions[index3 * 3 + 1], positions[index3 * 3 + 2]);

            glm::vec3 worldV1 = glm::vec3(worldMatrix * glm::vec4(v1, 1.0f));
            glm::vec3 worldV2 = glm::vec3(worldMatrix * glm::vec4(v2, 1.0f));
            glm::vec3 worldV3 = glm::vec3(worldMatrix * glm::vec4(v3, 1.0f));

            glm::vec3 hit;
            if(rayTriangleIntersection(rayOrigin, rayDirection, worldV1, worldV2, worldV3, hit)) {
                // if normal point in the same way as the ray direction, then it is a back face, ignore it
                glm::vec3 normal = glm::cross(worldV2 - worldV1, worldV3 - worldV1);
                if(glm::dot(normal, rayDirection) >= 0) {
                    continue;
                }

                PickResult result;
                result.hasHit = true;
                result.hitPoint = hit;
                result.faceIndex = i / 3;
                results.push_back(result);
            }
        }

        // find the closest hitPoint
        if(results.empty()) {
            return {};
        }

        auto closestResult = results[0];
        for(int i = 1; i < results.size(); i++) {
            if(glm::distance(rayOrigin, results[i].hitPoint) < glm::distance(rayOrigin, closestResult.hitPoint)) {
                closestResult = results[i];
            }
        }

        return closestResult;
    }
};

#endif //FEATHERGL_UTILS_H
