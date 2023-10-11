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

    PhysicsBody *addMesh(Mesh *mesh, float mass) {
        auto physicsBody = new PhysicsBody(mesh, mass);
        _physicsBodies.push_back(physicsBody);

        return physicsBody;
    }

    void removeMesh(Mesh *mesh) {
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

    void applyForce(Mesh *mesh, glm::vec3 force) {
        for (auto body: _physicsBodies) {
            if (body->mesh() == mesh) {
                body->applyForce(force);
                break;
            }
        }
    }

    void solve(float deltaTime) {
        for (auto body: _physicsBodies) {
            for (auto particle: body->particles()) {
                particle->velocity += deltaTime * particle->invMass * particle->forces();
            }
        }

        for (auto body: _physicsBodies) {
            for (auto particle: body->particles()) {
                particle->predictedPosition = particle->position + deltaTime * particle->velocity;
            }
        }

        for (auto body: _physicsBodies) {
            for (auto particle: body->particles()) {
                // generate collision constraints
            }
        }

        for (unsigned int i = 0; i < _iterations; i++) {
            // project constraints
        }

        for (auto body: _physicsBodies) {
            for (auto particle: body->particles()) {
                particle->velocity = (particle->predictedPosition - particle->position) / deltaTime;
                particle->position = particle->predictedPosition;

                // update actual mesh vertex data
                body->mesh()->vertexData().positions[particle->startIndex] = particle->position.x;
                body->mesh()->vertexData().positions[particle->startIndex + 1] = particle->position.y;
                body->mesh()->vertexData().positions[particle->startIndex + 2] = particle->position.z;
            }

            body->mesh()->updateVertexData();
        }
    }

private:
    int _iterations = 8;
    std::vector<PhysicsBody *> _physicsBodies;
};

#endif //FEATHERGL_HPBDSOLVER_H
