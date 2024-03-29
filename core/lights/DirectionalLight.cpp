//
// Created by barth on 27/09/2022.
//

#include "DirectionalLight.h"

DirectionalLight::DirectionalLight(const char *name, float x, float y, float z) : Light(name) {
    _direction = glm::normalize(glm::vec3(x, y, z));
}
