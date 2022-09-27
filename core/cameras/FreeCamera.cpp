//
// Created by barth on 27/09/2022.
//

#include "FreeCamera.h"

FreeCamera::FreeCamera(GLFWwindow *window) : Camera(window) {}

glm::mat4 FreeCamera::computeViewMatrix() {
    return glm::lookAt(_position, _position + getForwardDirection(), getUpwardDirection());
}

void FreeCamera::update() {
    _viewMatrix = computeViewMatrix();
    _projectionMatrix = computeProjectionMatrix();

    float speed = 0.02f;
    if (glfwGetKey(_window, GLFW_KEY_UP)) _position.z -= speed;
    if (glfwGetKey(_window, GLFW_KEY_DOWN)) _position.z += speed;
    if (glfwGetKey(_window, GLFW_KEY_LEFT)) _position.x -= speed;
    if (glfwGetKey(_window, GLFW_KEY_RIGHT)) _position.x += speed;
    if (glfwGetKey(_window, GLFW_KEY_SPACE)) _position.y += speed;
    if (glfwGetKey(_window, GLFW_KEY_LEFT_SHIFT)) _position.y -= speed;
}
