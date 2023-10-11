//
// Created by barth on 27/09/2022.
//

#ifndef FEATHERGL_TRANSFORM_H
#define FEATHERGL_TRANSFORM_H

#include <vector>
#include "glm/glm.hpp"

class Transform {
public:
    Transform();

    glm::vec3 *position();

    glm::vec3 getForwardDirection();

    glm::vec3 getUpwardDirection();

    glm::vec3 getLeftDirection();

    void setPosition(float x, float y, float z);

    void translate(glm::vec3 displacement);

    void setParent(Transform *parent);

    glm::vec3 getAbsolutePosition();

    void setRotationX(float angle) { _rotation->x = angle; };

    void setRotationY(float angle) { _rotation->y = angle; };

    void setRotationZ(float angle) { _rotation->z = angle; };

    void setRotation(float x, float y, float z) {
        setRotationX(x);
        setRotationY(y);
        setRotationZ(z);
    }

    glm::mat4 computeWorldMatrix();

    glm::mat4 computeNormalMatrix() {
        return glm::transpose(glm::inverse(computeWorldMatrix()));
    }

    void setScale(float scale);

    /**
     * Reset the transform to its default values (position: 0,0,0, rotation: 0,0,0, scale: 1)
     */
    void reset() {
        setPosition(0, 0, 0);
        setRotation(0, 0, 0);
        setScale(1);
    }

protected:
    glm::vec3 *_position = new glm::vec3(0.0);
    glm::vec3 *_scaling = new glm::vec3(1.0);
    glm::vec3 *_rotation = new glm::vec3(0.0);
    Transform *_parent = nullptr;

};


#endif //FEATHERGL_TRANSFORM_H
