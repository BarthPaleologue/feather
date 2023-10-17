//
// Created by barth on 17/10/23.
//

#ifndef FEATHERGL_SHADOWRENDERER_H
#define FEATHERGL_SHADOWRENDERER_H

#include "glad/glad.h"

class ShadowRenderer {
public:
    explicit ShadowRenderer(const unsigned int shadowMapWidth = 1024, const unsigned int shadowMapHeight = 1024)
            : _width(
            shadowMapWidth), _height(shadowMapHeight) {

        glGenFramebuffers(1, &_depthMapFBO);

        glGenTextures(1, &_depthMap);
        glBindTexture(GL_TEXTURE_2D, _depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT,
                     (int) _width, (int) _height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, _depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void bind() {
        glViewport(0, 0, (int) _width, (int) _height);
        glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        //ConfigureShaderAndMatrices();
        //RenderScene();
    }

    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

private:
    unsigned int _depthMapFBO{};
    unsigned int _depthMap{};
    unsigned int _width{};
    unsigned int _height{};
};

#endif //FEATHERGL_SHADOWRENDERER_H
