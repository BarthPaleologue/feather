//
// Created by barth on 27/09/23.
//

#ifndef FEATHERGL_TRANSFORMABLE_H
#define FEATHERGL_TRANSFORMABLE_H

#include "Transform.h"

class Transformable {
public:
    virtual Transform *transform() = 0;
};

#endif //FEATHERGL_TRANSFORMABLE_H
