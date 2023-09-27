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
    unsigned int outputTexture;

    PostProcessingShader *_shader;

    GLuint uniformScreenResolution;

    static const float rectangleVerts[];

    unsigned int rectVAO;
public:
    PostProcessing(int _width, int _height, const char *shaderFolder);

    void StartProcessing();

    void RenderTo(int targetFrameBuffer);

    void RenderToScreen() {
        RenderTo(0);
    }

    void setFBO(unsigned int FBO) {
        this->FBO = FBO;
    }

    unsigned int getFBO() const {
        return FBO;
    }

    void setRBO(unsigned int RBO) {
        this->RBO = RBO;
    }

    unsigned int getRBO() const {
        return RBO;
    }

    void setFramebufferTexture(unsigned int framebufferTexture) {
        this->outputTexture = framebufferTexture;
    }

    unsigned int getFramebufferTexture() const {
        return outputTexture;
    }

    void resize(int width, int height);

    ~PostProcessing() = default;
};

#endif //FEATHERGL_POSTPROCESSING_H