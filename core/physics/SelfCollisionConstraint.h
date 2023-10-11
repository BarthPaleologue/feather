//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_SELFCOLLISIONCONSTRAINT_H
#define FEATHERGL_SELFCOLLISIONCONSTRAINT_H

#include <glm/geometric.hpp>
#include "Constraint.h"

class SelfCollisionConstraint : public Constraint {
public:
    SelfCollisionConstraint(Particle *q, Particle *p1, Particle *p2, Particle *p3, float thickness) {
        _particles.push_back(q);
        _particles.push_back(p1);
        _particles.push_back(p2);
        _particles.push_back(p3);
        _cardinality = 4;
        _stiffness = 1;
        _type = UNILATERAL;
        _thickness = thickness;
    }

private:
    float _thickness;

    float evaluate() const override {
        glm::vec3 q = _particles[0]->predictedPosition;
        glm::vec3 p1 = _particles[1]->predictedPosition;
        glm::vec3 p2 = _particles[2]->predictedPosition;
        glm::vec3 p3 = _particles[3]->predictedPosition;

        glm::vec3 n = glm::normalize(glm::cross(p2 - p1, p3 - p1));
        glm::vec3 v = q - p1;

        return glm::dot(v, n) - _thickness;
    }
};

#endif //FEATHERGL_SELFCOLLISIONCONSTRAINT_H