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

    float evaluate() const override {
        return glm::length(_particles[0]->predictedPosition - _particles[1]->predictedPosition) - _l0;
    }
};

#endif //FEATHERGL_STRETCHCONSTRAINT_H
