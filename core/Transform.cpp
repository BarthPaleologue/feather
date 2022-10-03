//
// Created by barth on 27/09/2022.
//

#include "Transform.h"

Transform::Transform() {
    _position = new glm::vec3(0.0f);
    _rotation = new glm::vec3(0.0f);
}

void Transform::setPositionFromFloats(float x, float y, float z) {
    _position->x = x;
    _position->y = y;
    _position->z = z;
}

glm::vec3 *Transform::getPosition() {
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

void Transform::setParent(Transform *parent) {
    _parent = parent;
}

glm::vec3 Transform::getAbsolutePosition() {
    if(_parent == nullptr) return *_position;
    return *_position + _parent->getAbsolutePosition();
}
