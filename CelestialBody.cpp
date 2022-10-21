//
// Created by barth on 10/10/2022.
//

#include "CelestialBody.h"

CelestialBody::CelestialBody(const char *name, float radius, float rotationPeriod, float orbitPeriod, float orbitRadius, StandardMaterial *material): Sphere(name, radius, 32) {
    _radius = radius;
    _rotationPeriod = rotationPeriod;
    _orbitPeriod = orbitPeriod;
    _orbitRadius = orbitRadius;
    _material = material;
}

void CelestialBody::update(float time) {
    if(_orbitPeriod != 0) _position->x = _orbitRadius * std::cos(time / _orbitPeriod);
    _position->y = 0.0f;
    if(_orbitPeriod != 0) _position->z = _orbitRadius * std::sin(time / _orbitPeriod);

    if(_rotationPeriod != 0) setRotationY(-time / _rotationPeriod);
}