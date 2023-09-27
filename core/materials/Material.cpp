//
// Created by barth on 19/09/2022.
//

#include <iostream>
#include "glm/ext.hpp"
#include "Material.h"
#include "glad/glad.h"

Material::Material(const char *shaderFolder) {
    std::string vertexPath;
    vertexPath.append(shaderFolder);
    vertexPath += "/vertex.glsl";

    std::string fragmentPath;
    fragmentPath.append(shaderFolder);
    fragmentPath += "/fragment.glsl";

    loadFileToBuffer(vertexPath.c_str(), _vertexShaderCode);
    loadFileToBuffer(fragmentPath.c_str(), _fragmentShaderCode);
}

void Material::compile() {
    _program = glCreateProgram();

    std::string versionedVertexShaderCode = "#version 400\n" + _vertexShaderCode;
    std::string versionedFragmentShaderCode = "#version 400\n" + _fragmentShaderCode;


    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexShaderCString = versionedVertexShaderCode.c_str();
    glShaderSource(vs, 1, &vertexShaderCString, nullptr);
    glCompileShader(vs);

    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vs, 512, nullptr, infoLog);
        std::cout << "ERROR in compiling GL_VERTEX_SHADER\n" << infoLog << std::endl;
    }


    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragmentShaderCString = versionedFragmentShaderCode.c_str();
    glShaderSource(fs, 1, &fragmentShaderCString, nullptr);
    glCompileShader(fs);

    glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fs, 512, nullptr, infoLog);
        std::cout << "ERROR in compiling GL_FRAGMENT_SHADER\n" << infoLog << std::endl;
    }

    glAttachShader(_program, fs);
    glAttachShader(_program, vs);
    glLinkProgram(_program);
}

void Material::bind() {
    glUseProgram(_program);

    if (_isBackFaceCullingEnabled) {
        glEnable(GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
    } else {
        glDisable(GL_CULL_FACE);
    }

    glPolygonMode(GL_FRONT_AND_BACK, _isWireframe ? GL_LINE : GL_FILL);

    onBindObservable.notifyObservers();
}

void Material::setMat4(const char *uniformName, const glm::mat4 *matrix) const {
    glUniformMatrix4fv(glGetUniformLocation(_program, uniformName), 1, GL_FALSE, glm::value_ptr(*matrix));
}

void Material::setVec3(const char *uniformName, const glm::vec3 *vector) const {
    glUniform3fv(glGetUniformLocation(_program, uniformName), 1, glm::value_ptr(*vector));
}

void Material::bindTexture(const char *uniformName, Texture *texture, int id) const {
    glUniform1i(glGetUniformLocation(_program, uniformName), id);
    texture->bind(id);
}

void Material::unbind() {}

void Material::setDefine(const char *defineName) {
    _fragmentShaderCode = "#define " + std::string(defineName) + "\n" + _fragmentShaderCode;
    _vertexShaderCode = "#define " + std::string(defineName) + "\n" + _vertexShaderCode;
}

void Material::setInt(const char *uniformName, int integer) const {
    glUniform1i(glGetUniformLocation(_program, uniformName), integer);
}

void Material::setFloat(const char *uniformName, float value) const {
    glUniform1f(glGetUniformLocation(_program, uniformName), value);
}