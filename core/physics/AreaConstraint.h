//
// Created by barth on 15/11/23.
//

#ifndef FEATHERGL_AREACONSTRAINT_H
#define FEATHERGL_AREACONSTRAINT_H

#include <glm/geometric.hpp>
#include "Particle.h"
#include "Constraint.h"

class AreaConstraint : public Constraint {
public:
    AreaConstraint(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2, std::shared_ptr<Particle> p3, float a0) : Constraint(
            {p1, p2, p3}, 0.7, EQUALITY), _a0(a0) {};

private:
    float _a0;

    float evaluate() const override {
        glm::vec3 p1 = _particles[0]->predictedPosition;
        glm::vec3 p2 = _particles[1]->predictedPosition;
        glm::vec3 p3 = _particles[2]->predictedPosition;

        glm::vec3 cross = glm::cross(p2 - p1, p3 - p1);

        return glm::length(cross) / 2.0f - _a0;
    }

    void computeGradient() override {
        //TODO: implement
    }
};

#endif //FEATHERGL_AREACONSTRAINT_H
