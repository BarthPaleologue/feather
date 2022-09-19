//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_MATERIAL_H
#define FEATHERGL_MATERIAL_H


#include <string>
#include "glad/glad.h"

class Material {
public:
    explicit Material(const char* shaderFolder);
    void compile();
    void bind();
private:
    GLuint _program;
    std::string _vertexShaderCode;
    std::string _fragmentShaderCode;
};


#endif //FEATHERGL_MATERIAL_H
