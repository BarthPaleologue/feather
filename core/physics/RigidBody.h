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
        // iterate over all triangles and create distance constraints
        for(unsigned int i = 0; i < mesh->vertexData().indices.size(); i+= 3) {
            auto index1 = mesh->vertexData().indices[i];
            auto index2 = mesh->vertexData().indices[i + 1];
            auto index3 = mesh->vertexData().indices[i + 2];
            auto p1 = _particles[index1];
            auto p2 = _particles[index2];
            auto p3 = _particles[index3];
            _constraints.push_back(new DistanceConstraint(p1, p2, glm::length(p1->position - p2->position), 1.0f));
            _constraints.push_back(new DistanceConstraint(p2, p3, glm::length(p2->position - p3->position), 1.0f));
            _constraints.push_back(new DistanceConstraint(p3, p1, glm::length(p3->position - p1->position), 1.0f));
        }

        // find particles that share their positions and create distance constraints
        for(unsigned int i = 0; i < _particles.size(); i++) {
            for(unsigned int j = i + 1; j < _particles.size(); j++) {
                if(_particles[i]->position == _particles[j]->position) {
                    _constraints.push_back(new DistanceConstraint(_particles[i], _particles[j], 0.0f, 1.0f));
                }
            }
        }
    }
};

#endif //FEATHERGL_RIGIDBODY_H
