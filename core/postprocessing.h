#ifndef FEATHERGL_POSTPROCESSING_H
#define FEATHERGL_POSTPROCESSING_H

#include "glad/glad.h"

#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include "utils.h"
#include "Shader.h"

class PostProcessingShader : public Shader {
public:
    explicit PostProcessingShader(const char *shaderFolder) : Shader(shaderFolder) {};

    // Using a _program program
    void bind() override {
        Shader::bind();
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    ~PostProcessingShader() = default;
};

class PostProcessing {
private:
    int _width, _height;

    unsigned int FBO;
    unsigned int RBO;
    unsigned int framebufferTexture;

    PostProcessingShader *framebuffer_shader;

    GLuint uniformScreenResolution;

    static const float rectangleVerts[];

    unsigned int rectVAO;
public:
    PostProcessing(int _width, int _height, const char *shaderFolder);

    void StartProcessing();

    void EndProcessing();

    void resize(int width, int height);

    ~PostProcessing() = default;
};

#endif //FEATHERGL_POSTPROCESSING_H