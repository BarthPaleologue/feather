//
// Created by barth on 27/09/2022.
//

#include "OrbitCamera.h"

#include <cmath>

OrbitCamera::OrbitCamera(GLFWwindow *window) : Camera(window), _target(0.0f), _radius(20.0f), _theta(3.14f / 2.0f),
                                               _phi(0.0f) {}

glm::mat4 OrbitCamera::computeViewMatrix() {
    return glm::lookAt(*_position, _target, getUpwardDirection());
}

void OrbitCamera::update() {
    Camera::update();
    _viewMatrix = computeViewMatrix();

    float speed = 0.02f;
    if (glfwGetKey(_window, GLFW_KEY_UP)) _theta = std::max(_theta - speed, 0.14f);
    if (glfwGetKey(_window, GLFW_KEY_DOWN)) _theta = std::min(_theta + speed, 3.00f);
    if (glfwGetKey(_window, GLFW_KEY_LEFT)) _phi -= speed;
    if (glfwGetKey(_window, GLFW_KEY_RIGHT)) _phi += speed;

    _position->x = _target.x + _radius * std::sin(_theta) * std::sin(_phi);
    _position->z = _target.z + _radius * std::sin(_theta) * std::cos(_phi);
    _position->y = _target.y + _radius * std::cos(_theta);
}

void OrbitCamera::zoom(float amount) {
    _radius = std::max(_radius - amount, _minRadius);
}

void OrbitCamera::setTarget(glm::vec3 *target) {
    _target.x = target->x;
    _target.y = target->y;
    _target.z = target->z;
}

void OrbitCamera::setRadius(float radius) {
    _radius = radius;
}

void OrbitCamera::rotatePhi(float angle) {
    _phi += angle;
}

void OrbitCamera::rotateTheta(float angle) {
    _theta = std::max(std::min(_theta + angle, 3.00f), 0.14f);
}

void OrbitCamera::setMinRadius(float minRadius) {
    _minRadius = minRadius;
}
