//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_MATERIAL_H
#define FEATHERGL_MATERIAL_H


#include <string>
#include "glad/glad.h"
#include "glm/ext/matrix_float4x4.hpp"

class Material {
public:
    explicit Material(const char* shaderFolder);
    void compile();
    void bind() const;

    void setMat4(const char* uniformName, const glm::mat4 * matrix) const;

private:
    GLuint _program;
    std::string _vertexShaderCode;
    std::string _fragmentShaderCode;
};


#endif //FEATHERGL_MATERIAL_H
