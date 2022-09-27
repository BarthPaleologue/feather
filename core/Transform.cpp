//
// Created by barth on 27/09/2022.
//

#include "Transform.h"

Transform::Transform() : _position(0.0f), _rotation(0.0f) {}

void Transform::setPosition(glm::vec3 &newPosition) {
    _position.x = newPosition.x;
    _position.y = newPosition.y;
    _position.z = newPosition.z;
}

void Transform::setPositionFromFloats(float x, float y, float z) {
    _position.x = x;
    _position.y = y;
    _position.z = z;
}

glm::vec3 Transform::getPosition() {
    return _position;
}

glm::vec3 Transform::getForwardDirection() {
    return {0, 0, -1};
}

glm::vec3 Transform::getUpwardDirection() {
    return {0, 1, 0};
}

glm::vec3 Transform::getLeftDirection() {
    return {1, 0, 0};
}
