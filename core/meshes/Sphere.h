//
// Created by barth on 26/09/2022.
//

#ifndef FEATHERGL_SPHERE_H
#define FEATHERGL_SPHERE_H


#include "Mesh.h"

class Sphere: public Mesh {
public:
    Sphere(const char* name, float radius, int resolution);
};


#endif //FEATHERGL_SPHERE_H
