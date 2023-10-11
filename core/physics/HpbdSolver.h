//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_HPBDSOLVER_H
#define FEATHERGL_HPBDSOLVER_H

#include <vector>
#include "Mesh.h"
#include "Particle.h"

class HpbdSolver {
public:
    HpbdSolver() = default;

    void addMesh(Mesh *mesh, float mass) {
        _meshes.push_back(*mesh);
        makeParticles(mesh, mass);
    }

    void removeMesh(Mesh *mesh) {
        // find index of mesh in meshes then erase in _meshes and _particles
        unsigned long index = 0;
        for (auto &m: _meshes) {
            if (m == *mesh) {
                _meshes.erase(_meshes.begin() + index);
                _particles.erase(_particles.begin() + index);
                break;
            }
            index++;
        }
    }

    void solve() {

    }

private:
    void makeParticles(Mesh *mesh, float mass) {
        _particles.emplace_back();
        unsigned long lastIndex = _particles.size() - 1;
        for (unsigned int i = 0; i < mesh->vertexData().positions.size(); i += 3) {
            _particles[lastIndex].emplace_back(mass, mesh->vertexData().positions, i);
        }
    }

    std::vector<Mesh> _meshes;
    std::vector<std::vector<Particle>> _particles;
};

#endif //FEATHERGL_HPBDSOLVER_H
