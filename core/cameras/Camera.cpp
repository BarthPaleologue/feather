//
// Created by barth on 26/09/2022.
//

#include "Camera.h"

Camera::Camera(Engine *engine) : Transform(), _engine(engine) {
    int width, height;
    glfwGetWindowSize(_engine->getWindow(), &width, &height);
    setAspectRatio(static_cast<float>(width) / static_cast<float>(height));

    setPositionFromFloats(0.0f, 0.0f, 7.0f);
    setNear(0.1);
    setFar(80.1);

    _projectionMatrix = computeProjectionMatrix();
}

void Camera::update() {
    _projectionMatrix = computeProjectionMatrix();
    _viewMatrix = computeViewMatrix();
}
