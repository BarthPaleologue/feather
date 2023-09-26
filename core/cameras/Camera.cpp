//
// Created by barth on 26/09/2022.
//

#include "Camera.h"

Camera::Camera(Engine *engine) : Transform(), _engine(engine) {
    int width, height;
    glfwGetWindowSize(_engine->getWindow(), &width, &height);
    setAspectRatio(static_cast<float>(width) / static_cast<float>(height));

    _projectionMatrix = computeProjectionMatrix();
}

void Camera::update() {
    _projectionMatrix = computeProjectionMatrix();
    _viewMatrix = computeViewMatrix();
}
