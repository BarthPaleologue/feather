//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_BENDCONSTRAINT_H
#define FEATHERGL_BENDCONSTRAINT_H

#include <glm/geometric.hpp>
#include "Constraint.h"
#include "Dense"

/**
 * @see https://carmencincotti.com/2022-09-05/the-most-performant-bending-constraint-of-xpbd/
 */
class BendConstraint : public DistanceConstraint {
public:
    BendConstraint(Particle *p0, Particle *p1, Particle *p2, Particle *p3, float stiffness)
            : DistanceConstraint(
            p2, p3, glm::length(p2->position - p3->position), stiffness) {
    };
};

#endif //FEATHERGL_BENDCONSTRAINT_H
