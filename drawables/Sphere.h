//
// Created by barth on 26/09/2022.
//

#ifndef FEATHERGL_SPHERE_H
#define FEATHERGL_SPHERE_H


#include "Drawable.h"

class Sphere: public Drawable {
public:
    Sphere(const char* name, float radius, int resolution);

private:
    float _radius;
};


#endif //FEATHERGL_SPHERE_H
