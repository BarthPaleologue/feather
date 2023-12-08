//
// Created by barth on 08/12/23.
//

#ifndef FEATHERGL_RIGIDBODY_H
#define FEATHERGL_RIGIDBODY_H

#include "PhysicsBody.h"
#include "MeshBuilder.h"
#include "VolumeConstraint.h"

class RigidBody: public PhysicsBody {
public:
    RigidBody(std::shared_ptr<Mesh> mesh, float mass) : PhysicsBody(mesh, mass) {
        // a rigidbody is a physics body where all particles are fixed relative to each other
        for (unsigned int i = 0; i < _particles.size(); i++) {
            std::cout << toString(_particles[i]->position) << std::endl;
            for (unsigned int j = i + 1; j < _particles.size(); j++) {
                glm::vec3 diff = _particles[i]->position - _particles[j]->position;
                float distance = glm::length(diff);
                _constraints.push_back(new DistanceConstraint(_particles[i], _particles[j], distance, 1.0f));
            }
        }
    }
};

#endif //FEATHERGL_RIGIDBODY_H
