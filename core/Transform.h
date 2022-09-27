//
// Created by barth on 27/09/2022.
//

#ifndef FEATHERGL_TRANSFORM_H
#define FEATHERGL_TRANSFORM_H

#include "glm/glm.hpp"

class Transform {
public:
    Transform();

    void setPosition(glm::vec3 &newPosition);

    glm::vec3 getPosition();

    glm::vec3 getForwardDirection();

    glm::vec3 getUpwardDirection();

    glm::vec3 getLeftDirection();

    void setPositionFromFloats(float x, float y, float z);

protected:
    glm::vec3 _position;
    glm::vec3 _rotation;
};


#endif //FEATHERGL_TRANSFORM_H
