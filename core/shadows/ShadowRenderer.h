//
// Created by barth on 17/10/23.
//

#ifndef FEATHERGL_SHADOWRENDERER_H
#define FEATHERGL_SHADOWRENDERER_H

#include "glad/glad.h"
#include "DirectionalLight.h"

class ShadowRenderer {
public:
    explicit ShadowRenderer(std::shared_ptr<DirectionalLight> directionalLight,
                            const unsigned int shadowMapWidth = 1024, const unsigned int shadowMapHeight = 1024)
            :
            _width(shadowMapWidth), _height(shadowMapHeight), _directionalLight(directionalLight) {

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
        // get original viewport size so that we can restore it once we're done
        GLint dims[4] = {0};
        glGetIntegerv(GL_VIEWPORT, dims);
        _initialWidth = dims[2];
        _initialHeight = dims[3];

        glViewport(0, 0, (int) _width, (int) _height);
        glBindFramebuffer(GL_FRAMEBUFFER, _depthMapFBO);
        glClear(GL_DEPTH_BUFFER_BIT);
        computeProjectionViewMatrix();
    }

    void unbind() {
        //writeDepthTextureFromGPUToFile(_depthMap, "shadowMap.png");
        glViewport(0, 0, _initialWidth, _initialHeight);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void computeProjectionViewMatrix() {
        float near_plane = 1.0f, far_plane = 7.5f;
        glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

        glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f),
                                          glm::vec3(0.0f, 0.0f, 0.0f),
                                          glm::vec3(0.0f, 1.0f, 0.0f));

        _projectionViewMatrix = lightProjection * lightView;
    }

    glm::mat4 projectionViewMatrix() {
        return _projectionViewMatrix;
    }

private:
    std::shared_ptr<DirectionalLight> _directionalLight;

    glm::mat4 _projectionViewMatrix;

    unsigned int _depthMapFBO{};
    unsigned int _depthMap{};

    unsigned int _width{};
    unsigned int _height{};

    int _initialWidth{};
    int _initialHeight{};
};

#endif //FEATHERGL_SHADOWRENDERER_H
