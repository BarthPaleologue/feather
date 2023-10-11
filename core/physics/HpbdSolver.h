//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_HPBDSOLVER_H
#define FEATHERGL_HPBDSOLVER_H

#include <vector>
#include "Mesh.h"
#include "Particle.h"
#include "Constraint.h"

class HpbdSolver {
public:
    HpbdSolver() = default;

    ~HpbdSolver() {
        for (auto &particles: _particles) {
            for (auto &particle: particles) {
                delete particle;
            }
        }
    }

    void addMesh(Mesh *mesh, float mass) {
        _meshes.push_back(mesh);
        makeParticles(mesh, mass);
    }

    void removeMesh(Mesh *mesh) {
        // find index of mesh in meshes then erase in _meshes and _particles
        unsigned long index = 0;
        for (auto &m: _meshes) {
            if (m == mesh) {
                _meshes.erase(_meshes.begin() + index);
                _particles.erase(_particles.begin() + index);
                break;
            }
            index++;
        }
    }

    void solve(float deltaTime) {
        for (auto &particles: _particles) {
            for (auto &particle: particles) {
                particle->velocity += deltaTime * particle->invMass * glm::vec3(0, -9.81, 0);
            }
        }

        for (auto &particles: _particles) {
            for (auto &particle: particles) {
                particle->predictedPosition = particle->position + deltaTime * particle->velocity;
            }
        }

        for (auto &particles: _particles) {
            for (auto &particle: particles) {
                // generate collision constraints
            }
        }

        for (unsigned int i = 0; i < _iterations; i++) {
            // project constraints
        }

        for (unsigned int i = 0; i < _particles.size(); i++) {
            auto mesh = _meshes[i];
            for (auto &particle: _particles[i]) {
                particle->velocity = (particle->predictedPosition - particle->position) / deltaTime;
                particle->position = particle->predictedPosition;

                // update actual mesh vertex data
                mesh->vertexData().positions[particle->startIndex] = particle->position.x;
                mesh->vertexData().positions[particle->startIndex + 1] = particle->position.y;
                mesh->vertexData().positions[particle->startIndex + 2] = particle->position.z;
            }

            mesh->updateVertexData();
        }
    }

private:
    void makeParticles(Mesh *mesh, float mass) {
        _particles.emplace_back();
        unsigned long lastIndex = _particles.size() - 1;
        for (unsigned int i = 0; i < mesh->vertexData().positions.size(); i += 3) {
            _particles[lastIndex].emplace_back(new Particle(mass, mesh->vertexData().positions, i));
        }
    }

    int _iterations = 8;
    std::vector<Mesh *> _meshes;
    std::vector<std::vector<Particle *>> _particles;

    std::vector<Constraint *> _fixedConstraints;
};

#endif //FEATHERGL_HPBDSOLVER_H
