//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_HXPBDSOLVER_H
#define FEATHERGL_HXPBDSOLVER_H

#include <vector>
#include "Mesh.h"
#include "Particle.h"
#include "Constraint.h"
#include "PhysicsBody.h"
#include "CollisionConstraint.h"
#include "UniformAccelerationField.h"
#include "AABBHelper.h"

class HXPbdSolver {
public:
    HXPbdSolver() = default;

    ~HXPbdSolver() = default;

    void addBody(std::shared_ptr<PhysicsBody> pBody) {
        if (pBody->mass() > 0) pBody->buildParticleHierarchy(3);
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

    std::vector<std::shared_ptr<PhysicsBody>> &physicsBodies() {
        return _physicsBodies;
    }

    int nbParticles() {
        int nbParticles = 0;
        for (const auto &body: _physicsBodies) {
            nbParticles += body->nbParticles();
        }
        return nbParticles;
    }

    void solve(float deltaTime) {
        onBeforeSolveObservable.notifyObservers();

        // apply force fields
        for (const auto &body: _physicsBodies) {
            if(!body->mesh()->isEnabled()) continue;
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
                if(!body->mesh()->isEnabled()) continue;
                for (const auto &particle: body->particles()) {
                    particle->velocity += subTimeStep * particle->invMass * particle->resultingForce();

                    // velocity damping
                    particle->velocity *= 0.999;
                }
            }

            // predict positions
            for (const auto &body: _physicsBodies) {
                if(!body->mesh()->isEnabled()) continue;
                for (const auto &particle: body->particles()) {
                    particle->predictedPosition = particle->position + subTimeStep * particle->velocity;
                }
            }

            for (const auto &body: _physicsBodies) {
                if(!body->mesh()->isEnabled()) continue;
                // delete memory of pointers to collision constraints
                for (auto collisionConstraint: body->collisionConstraints()) {
                    delete collisionConstraint;
                }
                body->collisionConstraints().clear();
            }

            for (int k_body = 0; k_body < _physicsBodies.size(); k_body++) {
                auto body = _physicsBodies[k_body];
                if(!body->mesh()->isEnabled()) continue;

                glm::mat4 bodyWorld = body->mesh()->transform()->computeWorldMatrix();

                for (int k_otherBody = k_body + 1; k_otherBody < _physicsBodies.size(); k_otherBody++) {
                    auto otherBody = _physicsBodies[k_otherBody];
                    if(!otherBody->mesh()->isEnabled()) continue;

                    glm::mat4 otherBodyWorld = otherBody->mesh()->transform()->computeWorldMatrix();

                    AABB *intersection = AABB::intersection(body->mesh()->aabb(), otherBody->mesh()->aabb());
                    if (intersection == nullptr) continue;

                    // find particles from both bodies that are in the intersection
                    std::vector<std::shared_ptr<Particle>> bodyParticlesInIntersection;
                    for (const auto &particle: body->particles()) {
                        if (!intersection->contains(particle->predictedPosition)) continue;
                        bodyParticlesInIntersection.push_back(particle);
                    }

                    std::vector<std::shared_ptr<Particle>> otherBodyParticlesInIntersection;
                    for (const auto &particle: otherBody->particles()) {
                        if (!intersection->contains(particle->predictedPosition)) continue;
                        otherBodyParticlesInIntersection.push_back(particle);
                    }

                    // find triangles from both bodies that are in the intersection
                    std::vector<std::vector<GLint>> bodyTrianglesInIntersection;
                    std::vector<GLfloat> bodyPositions = body->mesh()->vertexData().positions;
                    std::vector<GLint> bodyIndices = body->mesh()->vertexData().indices;
                    for (unsigned int k = 0; k < bodyIndices.size(); k += 3) {
                        glm::vec3 t0 = body->particles()[bodyIndices[k]]->predictedPosition;
                        glm::vec3 t1 = body->particles()[bodyIndices[k + 1]]->predictedPosition;
                        glm::vec3 t2 = body->particles()[bodyIndices[k + 2]]->predictedPosition;

                        if (!intersection->intersectsTriangle(t0, t1, t2)) continue;

                        bodyTrianglesInIntersection.push_back({bodyIndices[k], bodyIndices[k + 1], bodyIndices[k + 2]});
                    }

                    std::vector<std::vector<GLint>> otherBodyTrianglesInIntersection;
                    std::vector<GLfloat> otherBodyPositions = otherBody->mesh()->vertexData().positions;
                    std::vector<GLint> otherBodyIndices = otherBody->mesh()->vertexData().indices;
                    for (unsigned int k = 0; k < otherBodyIndices.size(); k += 3) {
                        glm::vec3 t0 = otherBody->particles()[otherBodyIndices[k]]->predictedPosition;
                        glm::vec3 t1 = otherBody->particles()[otherBodyIndices[k + 1]]->predictedPosition;
                        glm::vec3 t2 = otherBody->particles()[otherBodyIndices[k + 2]]->predictedPosition;

                        if (!intersection->intersectsTriangle(t0, t1, t2)) continue;

                        otherBodyTrianglesInIntersection.push_back(
                                {otherBodyIndices[k], otherBodyIndices[k + 1], otherBodyIndices[k + 2]});
                    }

                    // for each particle in one body, create a collision constraint with each triangle in the other body (if there is a risk of collision)
                    for (const auto &particle: otherBodyParticlesInIntersection) {
                        for (const auto &triangle: bodyTrianglesInIntersection) {
                            glm::vec3 particleNormal = glm::vec3(
                                    otherBody->mesh()->vertexData().normals[particle->positionIndex],
                                    otherBody->mesh()->vertexData().normals[particle->positionIndex + 1],
                                    otherBody->mesh()->vertexData().normals[particle->positionIndex + 2]);
                            particleNormal = glm::normalize(glm::vec3(otherBodyWorld * glm::vec4(particleNormal, 0.0f)));

                            // if the trajectory of the particle does not intersect the triangle, skip this collision constraint
                            float t;
                            if(particle->mass != 0 && !Utils::rayTriangleIntersection(particle->predictedPosition - particleNormal * 0.1f, particleNormal,
                                                              body->particles()[triangle[0]]->predictedPosition,
                                                              body->particles()[triangle[1]]->predictedPosition,
                                                              body->particles()[triangle[2]]->predictedPosition, t)) continue;
                            if(t > 0.2f) continue;

                            auto collisionConstraint = new CollisionConstraint(particle,
                                                                               body->particles()[triangle[0]],
                                                                               body->particles()[triangle[1]],
                                                                               body->particles()[triangle[2]]);
                            otherBody->addCollisionConstraint(collisionConstraint);
                        }
                    }

                    for (const auto &particle: bodyParticlesInIntersection) {
                        for (const auto &triangle: otherBodyTrianglesInIntersection) {
                            glm::vec3 particleNormal = glm::vec3(
                                    body->mesh()->vertexData().normals[particle->positionIndex],
                                    body->mesh()->vertexData().normals[particle->positionIndex + 1],
                                    body->mesh()->vertexData().normals[particle->positionIndex + 2]);
                            particleNormal = glm::normalize(glm::vec3(bodyWorld * glm::vec4(particleNormal, 0.0f)));

                            // if the trajectory of the particle does not intersect the triangle, skip this collision constraint
                            float t;
                            if(particle->mass != 0 && !Utils::rayTriangleIntersection(particle->predictedPosition - particleNormal * 0.1f, particleNormal,
                                                               otherBody->particles()[triangle[0]]->predictedPosition,
                                                               otherBody->particles()[triangle[1]]->predictedPosition,
                                                               otherBody->particles()[triangle[2]]->predictedPosition, t)) continue;
                            if(t > 0.2f) continue;


                            auto collisionConstraint = new CollisionConstraint(particle,
                                                                               otherBody->particles()[triangle[0]],
                                                                               otherBody->particles()[triangle[1]],
                                                                               otherBody->particles()[triangle[2]]);
                            body->addCollisionConstraint(collisionConstraint);
                        }
                    }

                    delete intersection;
                }
            }

            // solve nonCollisionConstraints
            //for (unsigned int i = 0; i < _iterations; i++) {
            for (const auto &body: _physicsBodies) {
                if(!body->mesh()->isEnabled()) continue;

                for (const auto &distanceConstraints: body->distanceConstraintsPerLevel()) {
                    for (auto distanceConstraint: distanceConstraints) {
                        distanceConstraint->solve(subTimeStep);
                    }
                }
                for (auto bendConstraint: body->bendConstraints()) {
                    bendConstraint->solve(subTimeStep);
                }
                for (auto volumeConstraint: body->globalVolumeConstraints()) {
                    volumeConstraint->solve(subTimeStep);
                }
                for (auto collisionConstraint: body->collisionConstraints()) {
                    collisionConstraint->solve(subTimeStep);
                }
                for (auto fixedConstraint: body->fixedConstraints()) {
                    fixedConstraint->solve(subTimeStep);
                }
            }
            //}

            // tear edges apart when distance constraints are violated too much
            /*for (const auto &body: _physicsBodies) {
                for (const auto &distanceConstraints: body->distanceConstraintsPerLevel()) {
                    for (auto distanceConstraint: distanceConstraints) {
                        if(distanceConstraint->evaluate() > 0.3f) {
                            Utils::RemoveEdge(body->mesh()->vertexData().indices, distanceConstraint->particles()[0]->positionIndex / 3, distanceConstraint->particles()[1]->positionIndex / 3);

                            // for all non collision constraints, if they contain the two particles, set their stiffness to 0
                            //for(const auto &otherConstraint: body->nonCollisionConstraints()) {
                            //    if(otherConstraint == distanceConstraint) continue;
                            //    if(std::find(otherConstraint->particles().begin(), otherConstraint->particles().end(), distanceConstraint->particles()[0]) != otherConstraint->particles().end() &&
                            //       std::find(otherConstraint->particles().begin(), otherConstraint->particles().end(), distanceConstraint->particles()[1]) != otherConstraint->particles().end()) {
                            //        otherConstraint->setStiffness(0.0f);
                            //    }
                            //}
                        }
                    }
                }
            }*/

            for (const auto &body: _physicsBodies) {
                if(!body->mesh()->isEnabled()) continue;

                for (const auto &particle: body->particles()) {
                    particle->velocity = (particle->predictedPosition - particle->position) / subTimeStep;
                    particle->position = particle->predictedPosition;
                }
            }
        }

        // update mesh vertex data at the end of the simulation step
        for (const auto &body: _physicsBodies) {
            if(!body->mesh()->isEnabled()) continue;
            body->updateVertexData();
        }

        onAfterSolveObservable.notifyObservers();
    }

    void setIterations(int iterations) {
        _iterations = iterations;
    }

    Observable<> onBeforeSolveObservable;
    Observable<> onAfterSolveObservable;

private:
    int _iterations = 4;
    std::vector<std::shared_ptr<PhysicsBody>> _physicsBodies;
    std::vector<std::shared_ptr<UniformAccelerationField>> _fields;
};

#endif //FEATHERGL_HXPBDSOLVER_H
