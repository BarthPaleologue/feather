//
// Created by barth on 27/09/2022.
//

#include "PointLight.h"

PointLight::PointLight(const char *name): Light(name), _position(glm::vec3(0.0f)) {}
