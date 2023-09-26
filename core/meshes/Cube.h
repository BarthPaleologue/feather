//
// Created by barth on 27/09/2022.
//

#ifndef FEATHERGL_CUBE_H
#define FEATHERGL_CUBE_H

#include "Mesh.h"


class Cube: public Mesh {
public:
    Cube(const char *name, float x, float y, float z);
};


#endif //FEATHERGL_CUBE_H
