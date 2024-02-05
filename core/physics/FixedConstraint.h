//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_FIXEDCONSTRAINT_H
#define FEATHERGL_FIXEDCONSTRAINT_H

#include <glm/geometric.hpp>
#include <iostream>
#include "Particle.h"
#include "Constraint.h"

class FixedConstraint : public Constraint {
public:
    FixedConstraint(std::shared_ptr<Particle> p) : Constraint({p}, 1.0, EQUALITY), _targetPosition(p->position) {
        p->mass = 0.0f;
        p->invMass = 0.0f;
    };

    void setTargetPosition(glm::vec3 position) {
        _targetPosition = position;
    }

    glm::vec3 targetPosition() {
        return _targetPosition;
    }

private:
    glm::vec3 _targetPosition;

    void computeGradient() override {
        glm::vec3 p1 = _particles[0]->predictedPosition;
        glm::vec3 p2 = _targetPosition;

        glm::vec3 g1 = glm::normalize(p1 - p2);

        _gradient.col(0) = Eigen::Vector3f(g1.x, g1.y, g1.z);
    }

    float evaluate() const override {
        return glm::length(_particles[0]->predictedPosition - _targetPosition);
    }

    void recomputeTargetValue() override {
        _targetPosition = _particles[0]->predictedPosition;
    }
};

#endif //FEATHERGL_FIXEDCONSTRAINT_H
