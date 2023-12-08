//
// Created by barth on 08/12/23.
//

#ifndef FEATHERGL_SOFTBODY_H
#define FEATHERGL_SOFTBODY_H

#include "PhysicsBody.h"

class SoftBody: public PhysicsBody {
public:
    SoftBody(std::shared_ptr<Mesh> mesh, float mass) : PhysicsBody(mesh, mass) {
        // a softbody is a physics body where all particles are almost fixed to each other (and volume constraint)
        for (unsigned int i = 0; i < _particles.size(); i++) {
            for (unsigned int j = i + 1; j < _particles.size(); j++) {
                glm::vec3 diff = _particles[i]->position - _particles[j]->position;
                float distance = glm::length(diff);
                _constraints.push_back(new DistanceConstraint(_particles[i], _particles[j], distance, 0.0001f));
            }
        }
    }
};

#endif //FEATHERGL_SOFTBODY_H
