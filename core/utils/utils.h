//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_UTILS_H
#define FEATHERGL_UTILS_H

#include <string>
#include <vector>
#include <glm/vec3.hpp>
#include <iostream>
#include <glm/gtc/random.hpp>
#include <glm/ext/scalar_constants.hpp>
#include "glad/glad.h"
#include "Core"

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

    static void loopSubdivision(std::vector<float>& positions, std::vector<int>& indices) {
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
};

#endif //FEATHERGL_UTILS_H
