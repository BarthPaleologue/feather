//
// Created by barth on 26/09/2022.
//

#ifndef FEATHERGL_TRIANGLE_H
#define FEATHERGL_TRIANGLE_H

#include "AbstractMesh.h"

class Triangle: public AbstractMesh {
public:
    Triangle(const char* name, float x, float y, float z);
};


#endif //FEATHERGL_TRIANGLE_H
