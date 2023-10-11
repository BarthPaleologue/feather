//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_PHYSICSBODY_H
#define FEATHERGL_PHYSICSBODY_H

#include "Mesh.h"
#include "Particle.h"
#include "Constraint.h"

class PhysicsBody {
public:
    PhysicsBody(Mesh *mesh, float mass) : _mesh(mesh) {
        _particles.reserve(mesh->vertexData().positions.size() / 3);
        for (unsigned int i = 0; i < mesh->vertexData().positions.size(); i += 3) {
            _particles.push_back(new Particle(mass, mesh->vertexData().positions, i));
        }
    };

    std::vector<Particle *> &particles() {
        return _particles;
    }

    Mesh *mesh() {
        return _mesh;
    }

private:
    Mesh *_mesh;
    std::vector<Particle *> _particles;
    std::vector<Constraint *> _constraints;
};

#endif //FEATHERGL_PHYSICSBODY_H
