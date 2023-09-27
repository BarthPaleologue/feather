//
// Created by barth on 27/09/2022.
//

#ifndef FEATHERGL_POINTLIGHT_H
#define FEATHERGL_POINTLIGHT_H

#include <string>
#include "Light.h"
#include "glm/vec3.hpp"

class PointLight : public Light {
public:
    explicit PointLight(const char *name);

    const glm::vec3 *getPosition() { return &_position; }

    void setPosition(float x, float y, float z) {
        _position.x = x;
        _position.y = y;
        _position.z = z;
    }

    void setPosition(glm::vec3 position) { _position = position; }

private:
    glm::vec3 _position;
};


#endif //FEATHERGL_POINTLIGHT_H
