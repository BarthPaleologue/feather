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

    void solve() {

    }

private:
    void makeParticles(Mesh *mesh, float mass) {
        for (int i = 0; i < mesh->vertexData().positions.size(); i += 3) {
            _particles.emplace_back(mass, mesh->vertexData().positions[i],
                                    mesh->vertexData().positions[i + 1],
                                    mesh->vertexData().positions[i + 2]
            );
        }
    }

    std::vector<Mesh> _meshes;
    std::vector<Particle> _particles;
};

#endif //FEATHERGL_HPBDSOLVER_H
