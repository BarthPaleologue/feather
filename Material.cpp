//
// Created by barth on 19/09/2022.
//

#include "Material.h"
#include "utils.h"
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
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    const char* fragmentShaderCString = _fragmentShaderCode.c_str();
    glShaderSource(fs, 1, &fragmentShaderCString, nullptr);
    glCompileShader(fs);

    glAttachShader(_program, fs);
    glAttachShader(_program, vs);
    glLinkProgram(_program);
}

void Material::bind() {
    glUseProgram(_program);
}
