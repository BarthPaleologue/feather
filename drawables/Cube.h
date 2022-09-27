//
// Created by barth on 27/09/2022.
//

#ifndef FEATHERGL_CUBE_H
#define FEATHERGL_CUBE_H

#include "Drawable.h"


class Cube: public Drawable {
public:
    Cube(const char* name, int size, float x, float y, float z);
};


#endif //FEATHERGL_CUBE_H
