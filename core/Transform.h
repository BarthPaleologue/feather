//
// Created by barth on 27/09/2022.
//

#ifndef FEATHERGL_TRANSFORM_H
#define FEATHERGL_TRANSFORM_H

#include <vector>
#include <glm/ext/matrix_transform.hpp>
#include "glm/glm.hpp"

class Transform {
public:
    Transform();

    glm::vec3 position();

    glm::vec3 getForwardDirection();

    glm::vec3 getUpwardDirection();

    glm::vec3 getLeftDirection();

    void setPosition(float x, float y, float z);

    void setPosition(glm::vec3 position) {
        setPosition(position.x, position.y, position.z);
    }

    void translate(glm::vec3 displacement);

    void setParent(Transform *parent);

    glm::vec3 getAbsolutePosition();

    void setRotationX(float angle) { _rotation.x = angle; };

    void setRotationY(float angle) { _rotation.y = angle; };

    void setRotationZ(float angle) { _rotation.z = angle; };

    void setRotation(float x, float y, float z) {
        setRotationX(x);
        setRotationY(y);
        setRotationZ(z);
    }

    glm::mat4 computeWorldMatrix();

    glm::mat4 computeNormalMatrix() {
        return glm::transpose(glm::inverse(computeWorldMatrix()));
    }

    glm::mat4 computeRotationMatrix() {
        glm::mat4 rotation = glm::mat4(1.0f);
        rotation = glm::rotate(rotation, _rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        rotation = glm::rotate(rotation, _rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        rotation = glm::rotate(rotation, _rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        return rotation;
    }

    void setScale(float scale);

    void setScaling(glm::vec3 scale) {
        _scaling = scale;
    }

    glm::vec3 scaling() {
        return _scaling;
    }

    /**
     * Reset the transform to its default values (position: 0,0,0, rotation: 0,0,0, scale: 1)
     */
    void reset() {
        setPosition(0, 0, 0);
        setRotation(0, 0, 0);
        setScale(1);
    }

protected:
    glm::vec3 _position = glm::vec3(0.0);
    glm::vec3 _scaling = glm::vec3(1.0);
    glm::vec3 _rotation = glm::vec3(0.0);
    Transform *_parent = nullptr;

};


#endif //FEATHERGL_TRANSFORM_H
