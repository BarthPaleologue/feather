//
// Created by barth on 27/09/2022.
//

#include <GLFW/glfw3.h>
#include "FreeCamera.h"

FreeCamera::FreeCamera(Engine *engine) : Camera(engine) {
    engine->onKeyPressObservable.add([this](int key) {
        float speed = 0.02f;
        switch (key) {
            case GLFW_KEY_UP:
                _position->z -= speed;
                break;
            case GLFW_KEY_DOWN:
                _position->z += speed;
                break;
            case GLFW_KEY_LEFT:
                _position->x -= speed;
                break;
            case GLFW_KEY_RIGHT:
                _position->x += speed;
                break;
            case GLFW_KEY_SPACE:
                _position->y += speed;
                break;
            case GLFW_KEY_LEFT_SHIFT:
                _position->y -= speed;
                break;
            default:
                break;
        }
    });
}

glm::mat4 FreeCamera::computeViewMatrix() {
    return glm::lookAt(*_position, *_position + getForwardDirection(), getUpwardDirection());
}

void FreeCamera::update() {
    _viewMatrix = computeViewMatrix();
    _projectionMatrix = computeProjectionMatrix();
}
