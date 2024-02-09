//
// Created by barth on 02/01/24.
//

#ifndef FEATHERGL_UNIFORMACCELERATIONFIELD_H
#define FEATHERGL_UNIFORMACCELERATIONFIELD_H

#include <glm/vec3.hpp>

class UniformAccelerationField {
public:
    explicit UniformAccelerationField(glm::vec3 uniformAcceleration) : _acceleration(uniformAcceleration) {

    }

    glm::vec3 computeAcceleration() {
        return _acceleration;
    }

private:
    glm::vec3 _acceleration;
};

#endif //FEATHERGL_UNIFORMACCELERATIONFIELD_H
