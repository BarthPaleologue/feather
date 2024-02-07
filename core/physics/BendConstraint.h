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
    BendConstraint(std::shared_ptr<Particle> p0, std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2,
                   std::shared_ptr<Particle> p3, float compliance)
            : DistanceConstraint(p2, p3, compliance) {
    };
};

#endif //FEATHERGL_BENDCONSTRAINT_H
