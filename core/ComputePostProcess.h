//
// Created by barth on 13/10/23.
//

#ifndef FEATHERGL_COMPUTEPOSTPROCESS_H
#define FEATHERGL_COMPUTEPOSTPROCESS_H

#include "ComputeShader.h"

class ComputePostProcess : public ComputeShader {
public:
    ComputePostProcess(const char *shaderPath, glm::ivec2 size) : ComputeShader(shaderPath, size) {

    };

    void resize(unsigned int width, unsigned int height) {
        work_size = glm::ivec2(width, height);

        // resize framebuffer texture
        glBindTexture(GL_TEXTURE_2D, outputTexture.handle());
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, (int) width, (int) height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

        /*
        // resize render buffer
        glBindRenderbuffer(GL_RENDERBUFFER, RBO);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);

        // Error checking framebuffer
        auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
        if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "Framebuffer error: " << fboStatus << std::endl;*/
    }
};

#endif //FEATHERGL_COMPUTEPOSTPROCESS_H
