//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_HPBDSOLVER_H
#define FEATHERGL_HPBDSOLVER_H

#include <vector>
#include "Mesh.h"
#include "Particle.h"
#include "Constraint.h"
#include "PhysicsBody.h"

class HpbdSolver {
public:
    HpbdSolver() = default;

    ~HpbdSolver() {
        for (auto body: _physicsBodies) {
            for (auto particle: body->particles()) {
                delete particle;
            }
            delete body;
        }
    }

    PhysicsBody *addMesh(std::shared_ptr<Mesh> mesh, float mass) {
        auto physicsBody = new PhysicsBody(mesh, mass);
        _physicsBodies.push_back(physicsBody);

        return physicsBody;
    }

    void removeMesh(std::shared_ptr<Mesh> mesh) {
        // find index of mesh in meshes then erase in _meshes and _particles
        unsigned long index = 0;
        for (auto body: _physicsBodies) {
            if (body->mesh() == mesh) {
                _physicsBodies.erase(_physicsBodies.begin() + index);
                break;
            }
            index++;
        }
    }


    void addBody(PhysicsBody *pBody) {
        _physicsBodies.push_back(pBody);
    }

    void applyForcePerParticle(std::shared_ptr<Mesh> mesh, glm::vec3 force) {
        for (auto body: _physicsBodies) {
            if (body->mesh() == mesh) {
                body->applyForcePerParticle(force);
                break;
            }
        }
    }

    void solve(float deltaTime) {
        // this is coming from XPBD where the time step is divided into sub time steps at the top level
        float subTimeStep = deltaTime / (float) _iterations;
        for (unsigned int i = 0; i < _iterations; i++) {

            // apply forces
            for (auto body: _physicsBodies) {
                for (auto particle: body->particles()) {
                    particle->velocity += subTimeStep * particle->invMass * particle->forces();
                }
            }

            // predict positions
            for (auto body: _physicsBodies) {
                for (auto particle: body->particles()) {
                    particle->predictedPosition = particle->position + subTimeStep * particle->velocity;
                }
            }

            for (auto body: _physicsBodies) {
                for (auto particle: body->particles()) {
                    // generate collision constraints
                }
            }

            // HPBD
            for(auto body: _physicsBodies) {
                body->buildParticleHierarchy();
            }

            // solve constraints
            //for (unsigned int i = 0; i < _iterations; i++) {
            for (auto body: _physicsBodies) {
                for (auto constraint: body->constraints()) {
                    constraint->solve();
                }
            }
            //}

            for (auto body: _physicsBodies) {
                for (auto particle: body->particles()) {
                    particle->velocity = (particle->predictedPosition - particle->position) / subTimeStep;
                    particle->position = particle->predictedPosition;

                    // velocity damping
                    particle->velocity *= 0.999;

                    // update actual mesh vertex data
                    body->mesh()->vertexData().positions[particle->startIndex] = particle->position.x;
                    body->mesh()->vertexData().positions[particle->startIndex + 1] = particle->position.y;
                    body->mesh()->vertexData().positions[particle->startIndex + 2] = particle->position.z;
                }

                body->mesh()->vertexData().computeNormals();
                body->mesh()->sendVertexDataToGPU();
            }
        }
    }

private:
    int _iterations = 16;
    std::vector<PhysicsBody *> _physicsBodies;
};

#endif //FEATHERGL_HPBDSOLVER_H
