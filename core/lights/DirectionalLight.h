//
// Created by barth on 27/09/2022.
//

#ifndef FEATHERGL_DIRECTIONALLIGHT_H
#define FEATHERGL_DIRECTIONALLIGHT_H

#include "Light.h"
#include "glm/vec3.hpp"

class DirectionalLight : public Light {
public:
    DirectionalLight(const char* name, float x, float y, float z);

    inline glm::vec3 getDirection() { return _direction; }

    inline void setDirection(glm::vec3 direction) { _direction = direction; }

private:
    glm::vec3 _direction;
};


#endif //FEATHERGL_DIRECTIONALLIGHT_H
