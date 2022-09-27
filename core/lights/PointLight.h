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
    PointLight(const char *name);

    inline const glm::vec3 * getPosition() { return &_position; }

    inline void setPosition(glm::vec3 position) { _position = position; }

private:
    glm::vec3 _position;
};


#endif //FEATHERGL_POINTLIGHT_H
