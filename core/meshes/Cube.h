//
// Created by barth on 27/09/2022.
//

#ifndef FEATHERGL_CUBE_H
#define FEATHERGL_CUBE_H

#include "AbstractMesh.h"


class Cube: public AbstractMesh {
public:
    Cube(const char *name, float x, float y, float z);
};


#endif //FEATHERGL_CUBE_H
