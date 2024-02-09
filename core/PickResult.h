//
// Created by barth on 06/02/24.
//

#ifndef FEATHERGL_PICKRESULT_H
#define FEATHERGL_PICKRESULT_H

#include <memory>
#include <glm/vec3.hpp>

struct PickResult {
    bool hasHit = false;
    glm::vec3 hitPoint;
    int faceIndex;
};

#endif //FEATHERGL_PICKRESULT_H
