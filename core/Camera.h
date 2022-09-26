//
// Created by barth on 26/09/2022.
//

#ifndef FEATHERGL_CAMERA_H
#define FEATHERGL_CAMERA_H


#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "GLFW/glfw3.h"

class Camera {
public:
    inline float getFov() const { return _fov; }
    inline void setFoV(const float f) { _fov = f; }
    inline float getAspectRatio() const { return _aspectRatio; }
    inline void setAspectRatio(const float a) { _aspectRatio = a; }
    inline float getNear() const { return _near; }
    inline void setNear(const float n) { _near = n; }
    inline float getFar() const { return _far; }
    inline void setFar(const float n) { _far = n; }
    inline void setPosition(const glm::vec3 &p) { _position = p; }
    inline glm::vec3 getPosition() { return _position; }

    inline glm::mat4 computeViewMatrix() const {
        return glm::lookAt(_position, glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
    }

    // Returns the projection matrix stemming from the camera intrinsic parameter.
    inline glm::mat4 computeProjectionMatrix() const {
        return glm::perspective(glm::radians(_fov), _aspectRatio, _near, _far);
    }

    void onResize(GLFWwindow* window, int width, int height) {
        setAspectRatio(static_cast<float>(width)/static_cast<float>(height));
        glViewport(0, 0, (GLint)width, (GLint)height); // Dimension of the rendering region in the window
    }

private:
    glm::vec3 _position = glm::vec3(0, 0, 0);
    float _fov = 45.f;        // Field of view, in degrees
    float _aspectRatio = 1.f; // Ratio between the width and the height of the image
    float _near = 0.1f; // Distance before which geometry is excluded from the rasterization process
    float _far = 10.f; // Distance after which the geometry is excluded from the rasterization process
};


#endif //FEATHERGL_CAMERA_H
