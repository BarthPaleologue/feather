//
// Created by barth on 26/09/2022.
//

#ifndef FEATHERGL_CAMERA_H
#define FEATHERGL_CAMERA_H


#include "Transform.h"
#include "glm/ext.hpp"
#include "Engine.h"


class Camera : public Transform {
public:
    explicit Camera(Engine *engine);

    float getFov() const { return _fov; }

    void setFoV(const float f) { _fov = f; }

    float getAspectRatio() const { return _aspectRatio; }

    void setAspectRatio(const float a) { _aspectRatio = a; }

    float getNear() const { return _near; }

    void setNear(const float n) { _near = n; }

    float getFar() const { return _far; }

    void setFar(const float n) { _far = n; }

    virtual glm::mat4 computeViewMatrix() = 0;

    glm::mat4 viewMatrix() { return _viewMatrix; }

    // Returns the projection matrix stemming from the camera intrinsic parameter.
    glm::mat4 computeProjectionMatrix() const {
        return glm::perspective(glm::radians(_fov), _aspectRatio, _near, _far);
    }

    glm::mat4 projectionMatrix() { return _projectionMatrix; }

    glm::mat4 projectionViewMatrix() { return _projectionViewMatrix; }

    glm::vec3 getRayFromMouse(float mouseX, float mouseY, float windowWidth, float windowHeight) {
        glm::vec3 rayOrigin = position();

        glm::vec4 mouseClipPosition = glm::vec4((mouseX / windowWidth) * 2.0f - 1.0f,
                                                (1.0f - mouseY / windowHeight) * 2.0f - 1.0f, 0.0f, 1.0f);
        glm::vec4 mouseWorldPosition = glm::inverse(viewMatrix()) * glm::inverse(projectionMatrix()) *
                                       mouseClipPosition;
        mouseWorldPosition /= mouseWorldPosition.w;

        return glm::normalize(glm::vec3(mouseWorldPosition) - rayOrigin);
    }

    virtual void update();

protected:
    Engine *_engine;
    glm::mat4 _viewMatrix{};
    glm::mat4 _projectionMatrix{};
    glm::mat4 _projectionViewMatrix{};
    float _fov = 45.f;        // Field of view, in degrees
    float _aspectRatio = 1.f; // Ratio between the _width and the _height of the image
    float _near = 0.1f; // Distance before which geometry is excluded from the rasterization process
    float _far = 100.f; // Distance after which the geometry is excluded from the rasterization process
};


#endif //FEATHERGL_CAMERA_H
