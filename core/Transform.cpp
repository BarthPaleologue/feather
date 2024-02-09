//
// Created by barth on 27/09/2022.
//

#include "Transform.h"
#include "glm/ext.hpp"

Transform::Transform() {}

void Transform::setPosition(float x, float y, float z) {
    _position.x = x;
    _position.y = y;
    _position.z = z;
}

glm::vec3 Transform::position() {
    return _position;
}

glm::vec3 Transform::getForwardDirection() {
    glm::vec3 localForward = {0, 0, -1};
    return glm::normalize(glm::vec3(computeWorldMatrix() * glm::vec4(localForward, 0.0f)));
}

glm::vec3 Transform::getUpwardDirection() {
    glm::vec3 localUp = {0, 1, 0};
    return glm::normalize(glm::vec3(computeWorldMatrix() * glm::vec4(localUp, 0.0f)));
}

glm::vec3 Transform::getLeftDirection() {
    glm::vec3 localLeft = {-1, 0, 0};
    return glm::normalize(glm::vec3(computeWorldMatrix() * glm::vec4(localLeft, 0.0f)));
}

void Transform::setParent(Transform *parent) {
    _parent = parent;
}

glm::vec3 Transform::getAbsolutePosition() {
    if (_parent == nullptr) return _position;
    return _position + _parent->getAbsolutePosition();
}

glm::mat4 Transform::computeWorldMatrix() {
    glm::mat4 world = glm::mat4(1.0f);
    if(_parent != nullptr) world = world * _parent->computeWorldMatrix();
    world = glm::translate(world, _position);
    world = glm::scale(world, _scaling);
    world = glm::rotate(world, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
    world = glm::rotate(world, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
    world = glm::rotate(world, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

    return world;
}

void Transform::setScale(float scale) {
    _scaling.x = scale;
    _scaling.y = scale;
    _scaling.z = scale;
}

void Transform::translate(glm::vec3 displacement) {
    _position.x += displacement.x;
    _position.y += displacement.y;
    _position.z += displacement.z;
}
