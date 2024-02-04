//
// Created by barth on 27/09/23.
//

#ifndef FEATHERGL_SHADER_H
#define FEATHERGL_SHADER_H

#include <string>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <memory>
#include "glad/glad.h"
#include "Observable.h"
#include "Texture.h"

class Shader {
public:
    explicit Shader(const char *shaderFolder) {
        std::string vertexPath;
        vertexPath.append(shaderFolder);
        vertexPath += "/vertex.glsl";

        std::string fragmentPath;
        fragmentPath.append(shaderFolder);
        fragmentPath += "/fragment.glsl";

        loadFileToBuffer(vertexPath.c_str(), _vertexShaderCode);
        loadFileToBuffer(fragmentPath.c_str(), _fragmentShaderCode);

        onBeforeBindObservable.addOnce([this]() {
            compile();
        });
    }

    void compile() {
        _program = glCreateProgram();

        std::string versionedVertexShaderCode = "#version 430 core\n" + _vertexShaderCode;
        std::string versionedFragmentShaderCode = "#version 430 core\n" + _fragmentShaderCode;

        compileShader(versionedVertexShaderCode, GL_VERTEX_SHADER);
        compileShader(versionedFragmentShaderCode, GL_FRAGMENT_SHADER);

        GLint result = 0;
        GLchar errLog[1024] = {0};

        glLinkProgram(_program);
        glGetProgramiv(_program, GL_LINK_STATUS, &result);

        if (!result) {
            glGetProgramInfoLog(_program, sizeof(errLog), NULL, errLog);
            std::cerr << "Error linking program: '" << errLog << "'\n";
            return;
        }

        glValidateProgram(_program);
        glGetProgramiv(_program, GL_VALIDATE_STATUS, &result);

        if (!result) {
            glGetProgramInfoLog(_program, sizeof(errLog), NULL, errLog);
            std::cerr << "Error validating program: '" << errLog << "'\n";
            return;
        }
    }

    virtual void bind() {
        onBeforeBindObservable.notifyObservers();
        glUseProgram(_program);
        onAfterBindObservable.notifyObservers();
    }

    virtual void unbind() {}

    void setMat4(const char *uniformName, const glm::mat4 *matrix) const {
        glUniformMatrix4fv(glGetUniformLocation(_program, uniformName), 1, GL_FALSE, glm::value_ptr(*matrix));
    }

    void setVec2(const char *uniformName, const float x, const float y) const {
        glUniform2f(glGetUniformLocation(_program, uniformName), x, y);
    }

    void setVec2(const char *uniformName, const glm::vec2 vector) const {
        glUniform2fv(glGetUniformLocation(_program, uniformName), 1, glm::value_ptr(vector));
    }

    void setVec3(const char *uniformName, const glm::vec3 vector) const {
        glUniform3fv(glGetUniformLocation(_program, uniformName), 1, glm::value_ptr(vector));
    }

    void setInt(const char *uniformName, int integer) const {
        glUniform1i(glGetUniformLocation(_program, uniformName), integer);
    }

    void setFloat(const char *uniformName, float value) const {
        glUniform1f(glGetUniformLocation(_program, uniformName), value);
    }

    void setBool(const char *uniformName, bool value) const {
        glUniform1i(glGetUniformLocation(_program, uniformName), value);
    }

    void bindTexture(const char *uniformName, Texture *texture, int id) const {
        glUniform1i(glGetUniformLocation(_program, uniformName), id);
        texture->bind(id);
    }

    void setDefine(const char *defineName) {
        _fragmentShaderCode = "#define " + std::string(defineName) + "\n" + _fragmentShaderCode;
        _vertexShaderCode = "#define " + std::string(defineName) + "\n" + _vertexShaderCode;
    }

    GLuint program() const {
        return _program;
    }

    Observable<> onBeforeBindObservable{};
    Observable<> onAfterBindObservable{};
private:
    GLuint compileShader(std::string &shaderCode, GLenum shaderType) const {
        GLuint fs = glCreateShader(shaderType);
        const char *fragmentShaderCString = shaderCode.c_str();
        glShaderSource(fs, 1, &fragmentShaderCString, nullptr);
        glCompileShader(fs);

        GLint success;
        GLchar infoLog[512];
        glGetShaderiv(fs, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fs, 512, nullptr, infoLog);
            throw std::runtime_error("ERROR in compiling shader\n" + std::string(infoLog) + "\n" + shaderCode);
        }

        glAttachShader(_program, fs);

        return fs;
    }

    GLuint _program{};
    std::string _vertexShaderCode{};
    std::string _fragmentShaderCode{};
};

#endif //FEATHERGL_SHADER_H
