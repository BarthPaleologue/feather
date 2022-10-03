//
// Created by barth on 26/09/2022.
//

#ifndef FEATHERGL_CAMERA_H
#define FEATHERGL_CAMERA_H


#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "GLFW/glfw3.h"
#include "Transform.h"

class Camera : public Transform {
public:
    explicit Camera(GLFWwindow *window);

    inline float getFov() const { return _fov; }

    inline void setFoV(const float f) { _fov = f; }

    inline float getAspectRatio() const { return _aspectRatio; }

    inline void setAspectRatio(const float a) { _aspectRatio = a; }

    inline float getNear() const { return _near; }

    inline void setNear(const float n) { _near = n; }

    inline float getFar() const { return _far; }

    inline void setFar(const float n) { _far = n; }

    virtual inline glm::mat4 computeViewMatrix() = 0;

    inline glm::mat4 getViewMatrix() { return _viewMatrix; }

    // Returns the projection matrix stemming from the camera intrinsic parameter.
    inline glm::mat4 computeProjectionMatrix() {
        return glm::perspective(glm::radians(_fov), _aspectRatio, _near, _far);
    }

    inline glm::mat4 getProjectionMatrix() { return _projectionMatrix; }

    virtual void update();

protected:
    GLFWwindow *_window;
    glm::mat4 _viewMatrix{};
    glm::mat4 _projectionMatrix{};
    float _fov = 45.f;        // Field of view, in degrees
    float _aspectRatio = 1.f; // Ratio between the width and the height of the image
    float _near = 0.1f; // Distance before which geometry is excluded from the rasterization process
    float _far = 10.f; // Distance after which the geometry is excluded from the rasterization process
};


#endif //FEATHERGL_CAMERA_H
