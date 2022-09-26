//
// Created by barth on 19/09/2022.
//

#include <iostream>
#include "glm/ext.hpp"
#include "Material.h"
#include "../utils/utils.h"
#include "glad/glad.h"

Material::Material(const char* shaderFolder) {
    std::string vertexPath;
    vertexPath.append(shaderFolder);
    vertexPath += "/vertex.glsl";

    std::string fragmentPath;
    fragmentPath.append(shaderFolder);
    fragmentPath += "/fragment.glsl";

    loadFileToBuffer(vertexPath.c_str(), _vertexShaderCode);
    loadFileToBuffer(fragmentPath.c_str(), _fragmentShaderCode);
    _program = glCreateProgram();
    compile();
}

void Material::compile() {
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const char* vertexShaderCString = _vertexShaderCode.c_str();
    glShaderSource(vs, 1, &vertexShaderCString, nullptr);
    glCompileShader(vs);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(vs, 512, nullptr, infoLog);
        std::cout << "ERROR in compiling GL_VERTEX_SHADER\n" << infoLog << std::endl;
    }


    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderCString = _fragmentShaderCode.c_str();
    glShaderSource(fs, 1, &fragmentShaderCString, nullptr);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if(!success) {
        glGetShaderInfoLog(fs, 512, nullptr, infoLog);
        std::cout << "ERROR in compiling GL_FRAGMENT_SHADER\n" << infoLog << std::endl;
    }

    glAttachShader(_program, fs);
    glAttachShader(_program, vs);
    glLinkProgram(_program);
}

void Material::bind() const {
    glUseProgram(_program);
}

void Material::setMat4(const char *uniformName, const glm::mat4 *matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(_program, uniformName), 1, GL_FALSE, glm::value_ptr(*matrix));
}
