//
// Created by barth on 10/10/2022.
//

#include "CelestialBody.h"

CelestialBody::CelestialBody(const char *name, float radius, float rotationPeriod, float orbitPeriod, float orbitRadius,
                             std::shared_ptr<PhongMaterial> material, Scene &scene) :
        _mesh(MeshBuilder::makeSphere(name, scene, 32)) {
    _mesh->transform()->setScale(radius);
    _mesh->setMaterial(material);
    _rotationPeriod = rotationPeriod;
    _orbitPeriod = orbitPeriod;
    _orbitRadius = orbitRadius;
}

void CelestialBody::update(float time) {
    if (_orbitPeriod != 0) {
        _mesh->transform()->setPosition(
                _orbitRadius * std::cos(time / _orbitPeriod),
                0.0f,
                _orbitRadius * std::sin(time / _orbitPeriod)
        );
    }

    if (_rotationPeriod != 0) _mesh->transform()->setRotationY(-time / _rotationPeriod);
}