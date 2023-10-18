//
// Created by barth on 10/10/2022.
//

#ifndef FEATHERGL_CELESTIALBODY_H
#define FEATHERGL_CELESTIALBODY_H

#include "PhongMaterial.h"
#include "Scene.h"
#include "MeshBuilder.h"

class CelestialBody {
public:
    CelestialBody(const char *name, float radius, float rotationPeriod, float orbitPeriod, float orbitRadius,
                  PhongMaterial *material, Scene &scene);

    std::shared_ptr<Mesh> getMesh() { return _mesh; }

    void update(float time);

private:
    std::shared_ptr<Mesh> _mesh;
    float _rotationPeriod;
    float _orbitPeriod;
    float _orbitRadius;
};


#endif //FEATHERGL_CELESTIALBODY_H
