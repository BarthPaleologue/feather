//
// Created by barth on 26/09/2022.
//

#ifndef FEATHERGL_SPHERE_H
#define FEATHERGL_SPHERE_H


#include "Drawable.h"

class Sphere: public Drawable {
    Sphere(const char* name, int resolution, float x, float y, float z);
};


#endif //FEATHERGL_SPHERE_H
