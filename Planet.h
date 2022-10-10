//
// Created by barth on 10/10/2022.
//

#ifndef FEATHERGL_PLANET_H
#define FEATHERGL_PLANET_H

#include "Sphere.h"

class Planet: public Sphere {
public:
    Planet(const char* name, float radius, float rotationPeriod, float orbitPeriod, float orbitRadius);

    void update(float time);
private:
    float _rotationPeriod;
    float _orbitPeriod;
    float _orbitRadius;
};


#endif //FEATHERGL_PLANET_H
