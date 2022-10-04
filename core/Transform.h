//
// Created by barth on 27/09/2022.
//

#ifndef FEATHERGL_TRANSFORM_H
#define FEATHERGL_TRANSFORM_H

#include "glm/glm.hpp"

class Transform {
public:
    Transform();

    glm::vec3 *getPosition();

    glm::vec3 getForwardDirection();

    glm::vec3 getUpwardDirection();

    glm::vec3 getLeftDirection();

    void setPositionFromFloats(float x, float y, float z);

    void setParent(Transform *parent);

    glm::vec3 getAbsolutePosition();

    inline void setRotationX(float angle) { _rotation->x = angle; };

    inline void setRotationY(float angle) { _rotation->y = angle; };

    inline void setRotationZ(float angle) { _rotation->z = angle; };

protected:
    glm::vec3 *_position = new glm::vec3(0.0);
    glm::vec3 *_rotation = new glm::vec3(0.0);
    Transform *_parent = nullptr;

};


#endif //FEATHERGL_TRANSFORM_H
