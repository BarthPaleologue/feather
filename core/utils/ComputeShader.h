//
// Created by barth on 13/10/23.
//

#ifndef FEATHERGL_COMPUTESHADER_H
#define FEATHERGL_COMPUTESHADER_H

#include <glm/fwd.hpp>
#include <glm/detail/type_vec2.hpp>
#include <vector>
#include <iostream>
#include "glad/glad.h"
#include "utils.h"
#include "Texture.h"

class ComputeShader {
public:
    ComputeShader(const char *shaderPath, glm::ivec2 size) : work_size(size) {
        // compile shader
        std::string shaderCode;
        loadFileToBuffer(shaderPath, shaderCode);
        const char *shaderCodePtr = shaderCode.c_str();

        int compute_shader_id = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(compute_shader_id, 1, &shaderCodePtr, nullptr);
        glCompileShader(compute_shader_id);

        GLint success;
        glGetShaderiv(compute_shader_id, GL_COMPILE_STATUS, &success);
        if (!success) {
            char infoLog[512];
            glGetShaderInfoLog(compute_shader_id, 512, nullptr, infoLog);
            std::cout << "ERROR::SHADER::COMPUTE::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        id = glCreateProgram();
        glAttachShader(id, compute_shader_id);
        glLinkProgram(id);

        // clean up
        glDeleteShader(compute_shader_id);

        // generate texture
        outputTexture = new Texture(size.x, size.y);
        out_tex = outputTexture->handle();
    }

    ~ComputeShader() {
        glDeleteProgram(id);
        glDeleteTextures(1, &out_tex);
    }

    void set_values(float *values) {
        glBindTexture(GL_TEXTURE_2D, out_tex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, work_size.x, work_size.y, 0, GL_RGBA, GL_FLOAT, values);
    }

    std::vector<float> get_values() {
        unsigned int collection_size = work_size.x * work_size.y * 4;
        std::vector<float> compute_data(collection_size);
        glBindTexture(GL_TEXTURE_2D, out_tex);
        glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, compute_data.data());
        return compute_data;
    }

    void use() {
        glUseProgram(id);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, out_tex);
    }

    void dispatch() {
        // just keep it simple, 2d work group
        glDispatchCompute(work_size.x, work_size.y, 1);
    }

    void wait() {
        glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
    }

private:
    GLuint id;
    GLuint out_tex;
    glm::ivec2 work_size;

    Texture *outputTexture;
};

#endif //FEATHERGL_COMPUTESHADER_H
