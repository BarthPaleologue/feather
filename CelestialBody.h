//
// Created by barth on 10/10/2022.
//

#ifndef FEATHERGL_CELESTIALBODY_H
#define FEATHERGL_CELESTIALBODY_H

#include "Sphere.h"

class CelestialBody: public Sphere {
public:
    CelestialBody(const char* name, float radius, float rotationPeriod, float orbitPeriod, float orbitRadius);

    inline float getRadius() { return _radius; }

    void update(float time);
private:
    float _radius;
    float _rotationPeriod;
    float _orbitPeriod;
    float _orbitRadius;
};


#endif //FEATHERGL_CELESTIALBODY_H
