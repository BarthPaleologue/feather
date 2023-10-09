#ifndef FEATHERGL_POSTPROCESSING_H
#define FEATHERGL_POSTPROCESSING_H

#include "glad/glad.h"

#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include "utils.h"
#include "Shader.h"
#include "Engine.h"

class PostProcessing {
private:
    int _width, _height;

    unsigned int FBO;
    unsigned int RBO;
    unsigned int outputTexture;

    Shader *_shader;

    GLuint uniformScreenResolution;

    static const float rectangleVerts[];

    unsigned int rectVAO;
public:
    explicit PostProcessing(const char *shaderFolder, Engine *engine);

    void RenderTo(unsigned int targetFrameBuffer);

    void RenderToScreen() {
        RenderTo(0);
    }

    void setFBO(unsigned int FBO) {
        this->FBO = FBO;
    }

    unsigned int getFBO() const {
        return FBO;
    }

    void resize(int width, int height);

    Shader *shader() {
        return _shader;
    }

    Observable<> onBeforeRenderObservable;

    ~PostProcessing() = default;
};

#endif //FEATHERGL_POSTPROCESSING_H