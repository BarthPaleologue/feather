//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_FASTBENDCONSTRAINT_H
#define FEATHERGL_FASTBENDCONSTRAINT_H

#include <glm/geometric.hpp>
#include "Constraint.h"
#include "Dense"

/**
 * @see https://carmencincotti.com/2022-09-05/the-most-performant-bending-constraint-of-xpbd/
 */
class FastBendConstraint : public DistanceConstraint {
public:
    /**
     * Create a new FastBendConstraint using a distance constraint between p2 and p3
     * @param p0 The first particle of the shared edge between the two triangles
     * @param p1 The second particle of the shared edge between the two triangles
     * @param p2 The third particle of the first triangle
     * @param p3 The third particle of the second triangle
     * @param compliance The compliance of the constraint
     */
    FastBendConstraint(std::shared_ptr<Particle> p0, std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2,
                       std::shared_ptr<Particle> p3, float compliance)
            : DistanceConstraint(p2, p3, compliance) {
    };
};

#endif //FEATHERGL_FASTBENDCONSTRAINT_H
