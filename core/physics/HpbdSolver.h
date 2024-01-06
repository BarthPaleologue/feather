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
#include "CollisionConstraint.h"
#include "UniformAccelerationField.h"

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

    void addBody(PhysicsBody *pBody) {
        pBody->buildParticleHierarchy(1);
        _physicsBodies.push_back(pBody);
    }

    void addField(std::shared_ptr<UniformAccelerationField> field) {
        _fields.push_back(field);
    }

    void reset() {
        for (auto body: _physicsBodies) {
            body->reset();
        }
    }

    void solve(float deltaTime) {
        onBeforeSolveObservable.notifyObservers();

        // apply force fields
        for (auto body: _physicsBodies) {
            for (auto particle: body->particles()) {
                for (auto field: _fields) {
                    particle->forces.push_back(
                            field->computeAcceleration() * particle->mass);
                }
            }
        }

        // this is coming from XPBD where the time step is divided into sub time steps at the top level
        float subTimeStep = deltaTime / (float) _iterations;
        for (unsigned int i = 0; i < _iterations; i++) {
            // apply resultingForce
            for (auto body: _physicsBodies) {
                for (auto particle: body->particles()) {
                    particle->velocity += subTimeStep * particle->invMass * particle->resultingForce();
                }
            }

            // predict positions
            for (auto body: _physicsBodies) {
                for (auto particle: body->particles()) {
                    particle->predictedPosition = particle->position + subTimeStep * particle->velocity;
                }
            }

            // collision constraints
            /*auto ground1 = new Particle(0.0f, -1.0f, -3.0f, 0.0f);
            auto ground2 = new Particle(0.0f, 1.0f, -3.0f, 0.0f);
            auto ground3 = new Particle(0.0f, 0.0f, -3.0f, -1.0f);

            for (auto body: _physicsBodies) {
                body->collisionConstraints().clear();
                for (auto particle: body->particles()) {
                    auto constraint = new CollisionConstraint(particle, ground1, ground2, ground3, 0.1f);
                    body->constraints().push_back(constraint);
                    body->collisionConstraints().push_back(constraint);
                }
            }*/

            // HPBD
            /*for (auto body: _physicsBodies) {
                body->buildParticleHierarchy(1);
            }*/

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
                }
            }
        }

        for (auto body: _physicsBodies) {
            for (auto particle: body->particles()) {
                particle->forces.clear();

                auto particleLocalPosition = particle->position - *body->transform()->position();

                // update actual mesh vertex data
                body->mesh()->vertexData().positions[particle->positionIndex] = particleLocalPosition.x;
                body->mesh()->vertexData().positions[particle->positionIndex + 1] = particleLocalPosition.y;
                body->mesh()->vertexData().positions[particle->positionIndex + 2] = particleLocalPosition.z;
            }

            body->mesh()->vertexData().computeNormals();
            body->mesh()->sendVertexDataToGPU();
        }

        onAfterSolveObservable.notifyObservers();
    }

    Observable<> onBeforeSolveObservable;
    Observable<> onAfterSolveObservable;

private:
    int _iterations = 16;
    std::vector<PhysicsBody *> _physicsBodies;
    std::vector<std::shared_ptr<UniformAccelerationField>> _fields;
};

#endif //FEATHERGL_HPBDSOLVER_H
