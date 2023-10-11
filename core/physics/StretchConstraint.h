//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_STRETCHCONSTRAINT_H
#define FEATHERGL_STRETCHCONSTRAINT_H

#include <glm/geometric.hpp>
#include "Constraint.h"

class StretchConstraint : public Constraint {
public:
    StretchConstraint(Particle *p1, Particle *p2, float l0) : Constraint({p1, p2}, 1, BILATERAL) {
        _l0 = l0;
    }

private:
    float _l0;

    void computeGradient() override {
        glm::vec3 p1 = _particles[0]->predictedPosition;
        glm::vec3 p2 = _particles[1]->predictedPosition;

        glm::vec3 g1 = glm::normalize(p2 - p1);
        glm::vec3 g2 = -g1;

        _gradient.col(0) = Eigen::Vector3f(g1.x, g1.y, g1.z);
        _gradient.col(1) = Eigen::Vector3f(g2.x, g2.y, g2.z);
    }

    float evaluate() const override {
        return glm::length(_particles[0]->predictedPosition - _particles[1]->predictedPosition) - _l0;
    }
};

#endif //FEATHERGL_STRETCHCONSTRAINT_H
