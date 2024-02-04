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
#include "AABBHelper.h"

class HpbdSolver {
public:
    HpbdSolver() = default;

    ~HpbdSolver() = default;

    void addBody(std::shared_ptr<PhysicsBody> pBody) {
        if(pBody->mass() > 0) pBody->buildParticleHierarchy(3);
        else pBody->buildParticleHierarchy(1);
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
        for (const auto &body: _physicsBodies) {
            for (const auto &particle: body->particles()) {
                for (const auto &field: _fields) {
                    particle->forces.push_back(
                            field->computeAcceleration() * particle->mass);
                }
            }
        }

        // this is coming from XPBD where the time step is divided into sub time steps at the top level
        //float subTimeStep = deltaTime;
        float subTimeStep = deltaTime / (float) _iterations;
        for (unsigned int i = 0; i < _iterations; i++) {
            // apply resultingForce
            for (const auto &body: _physicsBodies) {
                for (const auto &particle: body->particles()) {
                    particle->velocity += subTimeStep * particle->invMass * particle->resultingForce();
                }
            }

            // predict positions
            for (const auto &body: _physicsBodies) {
                for (const auto &particle: body->particles()) {
                    particle->predictedPosition = particle->position + subTimeStep * particle->velocity;
                }
            }

            // collision nonCollisionConstraints
            /*auto ground1 = new Particle(0.0f, -1.0f, -3.0f, 0.0f);
            auto ground2 = new Particle(0.0f, 1.0f, -3.0f, 0.0f);
            auto ground3 = new Particle(0.0f, 0.0f, -3.0f, -1.0f);*/

            for (const auto& body: _physicsBodies) {
                body->collisionConstraints().clear();
            }

            for (int k_body = 0; k_body < _physicsBodies.size(); k_body++) {
                auto body = _physicsBodies[k_body];
                for(int k_otherBody = k_body + 1; k_otherBody < _physicsBodies.size(); k_otherBody++) {
                    auto otherBody = _physicsBodies[k_otherBody];

                    AABB* intersection = AABB::intersection(body->mesh()->aabb(), otherBody->mesh()->aabb());
                    if(intersection == nullptr) continue;

                    std::cout << "Found AABB intersection between " << body->mesh()->name() << " and " << otherBody->mesh()->name() << std::endl;

                    // find particles from both bodies that are in the intersection
                    std::vector<std::shared_ptr<Particle>> bodyParticlesInIntersection;
                    for(const auto& particle: body->particles()) {
                        if(!intersection->contains(particle->predictedPosition)) continue;
                        bodyParticlesInIntersection.push_back(particle);
                    }

                    std::vector<std::shared_ptr<Particle>> otherBodyParticlesInIntersection;
                    for(const auto& particle: otherBody->particles()) {
                        if(!intersection->contains(particle->predictedPosition)) continue;
                        otherBodyParticlesInIntersection.push_back(particle);
                    }

                    // find triangles from both bodies that are in the intersection
                    std::vector<std::vector<GLint>> bodyTrianglesInIntersection;
                    std::vector<GLfloat> bodyPositions = body->mesh()->vertexData().positions;
                    std::vector<GLint> bodyIndices = body->mesh()->vertexData().indices;
                    for(unsigned int k = 0; k < bodyIndices.size(); k += 3) {
                        glm::vec3 t0 = glm::vec3(bodyPositions[bodyIndices[k] * 3], bodyPositions[bodyIndices[k] * 3 + 1], bodyPositions[bodyIndices[k] * 3 + 2]);
                        glm::vec3 t1 = glm::vec3(bodyPositions[bodyIndices[k + 1] * 3], bodyPositions[bodyIndices[k + 1] * 3 + 1], bodyPositions[bodyIndices[k + 1] * 3 + 2]);
                        glm::vec3 t2 = glm::vec3(bodyPositions[bodyIndices[k + 2] * 3], bodyPositions[bodyIndices[k + 2] * 3 + 1], bodyPositions[bodyIndices[k + 2] * 3 + 2]);

                        if(!intersection->intersectsTriangle(t0, t1, t2)) continue;

                        bodyTrianglesInIntersection.push_back({bodyIndices[k], bodyIndices[k + 1], bodyIndices[k + 2]});
                    }

                    std::vector<std::vector<GLint>> otherBodyTrianglesInIntersection;
                    std::vector<GLfloat> otherBodyPositions = otherBody->mesh()->vertexData().positions;
                    std::vector<GLint> otherBodyIndices = otherBody->mesh()->vertexData().indices;
                    for(unsigned int k = 0; k < otherBodyIndices.size(); k += 3) {
                        glm::vec3 t0 = glm::vec3(otherBodyPositions[otherBodyIndices[k] * 3], otherBodyPositions[otherBodyIndices[k] * 3 + 1], otherBodyPositions[otherBodyIndices[k] * 3 + 2]);
                        glm::vec3 t1 = glm::vec3(otherBodyPositions[otherBodyIndices[k + 1] * 3], otherBodyPositions[otherBodyIndices[k + 1] * 3 + 1], otherBodyPositions[otherBodyIndices[k + 1] * 3 + 2]);
                        glm::vec3 t2 = glm::vec3(otherBodyPositions[otherBodyIndices[k + 2] * 3], otherBodyPositions[otherBodyIndices[k + 2] * 3 + 1], otherBodyPositions[otherBodyIndices[k + 2] * 3 + 2]);

                        if(!intersection->intersectsTriangle(t0, t1, t2)) continue;

                        otherBodyTrianglesInIntersection.push_back({otherBodyIndices[k], otherBodyIndices[k + 1], otherBodyIndices[k + 2]});
                    }

                    std::cout << "Found " << bodyParticlesInIntersection.size() << " particles in intersection" << std::endl;
                    std::cout << "Found " << otherBodyParticlesInIntersection.size() << " particles in intersection" << std::endl;

                    std::cout << "Found " << bodyTrianglesInIntersection.size() << " triangles in intersection" << std::endl;
                    std::cout << "Found " << otherBodyTrianglesInIntersection.size() << " triangles in intersection" << std::endl;

                    // create collision constraints
                    // for each particle from one body, create a collision constraint with every triangle from the other body
                    std::cout << "Creating " << bodyParticlesInIntersection.size() * otherBodyTrianglesInIntersection.size() << " collision constraints" << std::endl;
                    for(const auto& particle: bodyParticlesInIntersection) {
                        for(const auto& triangle: otherBodyTrianglesInIntersection) {
                            auto collisionConstraint = new CollisionConstraint(particle, otherBody->particles()[triangle[0]], otherBody->particles()[triangle[1]], otherBody->particles()[triangle[2]]);
                            body->collisionConstraints().push_back(collisionConstraint);
                        }
                    }

                    std::cout << "Creating " << otherBodyParticlesInIntersection.size() * bodyTrianglesInIntersection.size() << " collision constraints" << std::endl;
                    for(const auto& particle: otherBodyParticlesInIntersection) {
                        for(const auto& triangle: bodyTrianglesInIntersection) {
                            auto collisionConstraint = new CollisionConstraint(particle, body->particles()[triangle[0]], body->particles()[triangle[1]], body->particles()[triangle[2]]);
                            otherBody->collisionConstraints().push_back(collisionConstraint);
                        }
                    }

                    delete intersection;
                }
            }

            // solve nonCollisionConstraints
            //for (unsigned int i = 0; i < _iterations; i++) {
            for (const auto &body: _physicsBodies) {
                for (auto bendConstraint: body->bendConstraints()) {
                    bendConstraint->solve();
                }
                for (const auto &distanceConstraints: body->distanceConstraintsPerLevel()) {
                    for (auto distanceConstraint: distanceConstraints) {
                        distanceConstraint->solve();
                    }
                }
                for (auto bendConstraint: body->bendConstraints()) {
                    bendConstraint->solve();
                }
                for (auto fixedConstraint: body->fixedConstraints()) {
                    fixedConstraint->solve();
                }
                for (auto collisionConstraint: body->collisionConstraints()) {
                    collisionConstraint->solve();
                }
            }
            //}

            for (const auto &body: _physicsBodies) {
                for (const auto &particle: body->particles()) {
                    particle->velocity = (particle->predictedPosition - particle->position) / subTimeStep;
                    particle->position = particle->predictedPosition;

                    // velocity damping
                    particle->velocity *= 0.999;
                }
            }
        }

        // update mesh vertex data at the end of the simulation step
        for (const auto &body: _physicsBodies) {
            body->updateVertexData();
        }

        onAfterSolveObservable.notifyObservers();
    }

    Observable<> onBeforeSolveObservable;
    Observable<> onAfterSolveObservable;

private:
    int _iterations = 8;
    std::vector<std::shared_ptr<PhysicsBody>> _physicsBodies;
    std::vector<std::shared_ptr<UniformAccelerationField>> _fields;
};

#endif //FEATHERGL_HPBDSOLVER_H
