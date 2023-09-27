//
// Created by barth on 27/09/23.
//

#ifndef FEATHERGL_TRANSFORMABLE_H
#define FEATHERGL_TRANSFORMABLE_H

#include "Transform.h"

class Transformable {
public:
    Transform *transform() {
        return &_transform;
    }

private:
    Transform _transform{};
};

#endif //FEATHERGL_TRANSFORMABLE_H
