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

/**
 * Load file given by filename to the stringBuffer
 * @param filename
 * @param stringBuffer
 */
void loadFileToBuffer(const char *filename, std::string &stringBuffer);

GLuint loadTextureFromFileToGPU(const char *filename);

void writeTextureToPPM(GLuint textureHandle, const char *filename);

void writeDepthTextureToPPM(GLuint textureHandle, const char *filename);

std::string toString(glm::vec3 &vec);

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
        std::cout << theta << phi << std::endl;
        return {glm::sin(phi) * glm::cos(theta), glm::cos(phi), glm::sin(phi) * glm::sin(theta)};
    }
};

#endif //FEATHERGL_UTILS_H
