//
// Created by barth on 10/10/2022.
//

#ifndef FEATHERGL_CELESTIALBODY_H
#define FEATHERGL_CELESTIALBODY_H

#include "StandardMaterial.h"
#include "Scene.h"

class CelestialBody {
public:
    CelestialBody(const char *name, float radius, float rotationPeriod, float orbitPeriod, float orbitRadius,
                  StandardMaterial *material, Scene &scene);

    inline float getRadius() { return _radius; }

    Mesh *getMesh() { return _mesh; }

    void update(float time);

private:
    Mesh *_mesh;
    float _radius;
    float _rotationPeriod;
    float _orbitPeriod;
    float _orbitRadius;
};


#endif //FEATHERGL_CELESTIALBODY_H
