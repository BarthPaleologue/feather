//
// Created by barth on 27/09/2022.
//

#ifndef FEATHERGL_POINTLIGHT_H
#define FEATHERGL_POINTLIGHT_H

#include <string>
#include "Light.h"
#include "glm/vec3.hpp"
#include "Transformable.h"

class PointLight : public Light, public Transformable {
public:
    explicit PointLight(const char *name);
};


#endif //FEATHERGL_POINTLIGHT_H
