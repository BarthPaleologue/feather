//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_BENDCONSTRAINT_H
#define FEATHERGL_BENDCONSTRAINT_H

#include <glm/geometric.hpp>
#include "Constraint.h"

class BendConstraint : public Constraint {
public:
    BendConstraint(Particle *p1, Particle *p2, Particle *p3, Particle *p4, float phi) : Constraint(
            {p1, p2, p3, p4}, 1, BILATERAL), _phi(phi) {};

private:
    float _phi;

    void computeGradient() override {
        //TODO: implement
    }

    float evaluate() const override {
        glm::vec3 p1 = _particles[0]->predictedPosition;
        glm::vec3 p2 = _particles[1]->predictedPosition;
        glm::vec3 p3 = _particles[2]->predictedPosition;
        glm::vec3 p4 = _particles[3]->predictedPosition;

        glm::vec3 n1 = glm::normalize(glm::cross(p2 - p1, p3 - p1));
        glm::vec3 n2 = glm::normalize(glm::cross(p2 - p1, p4 - p1));

        return acosf(glm::dot(n1, n2)) - _phi;
    }
};

#endif //FEATHERGL_BENDCONSTRAINT_H
