//
// Created by barth on 15/11/23.
//

#ifndef FEATHERGL_VOLUMECONSTRAINT_H
#define FEATHERGL_VOLUMECONSTRAINT_H

#include <glm/geometric.hpp>
#include "Constraint.h"

class VolumeConstraint : public Constraint {
public:
    VolumeConstraint(Particle *p1, Particle *p2, Particle *p3, Particle *p4, float v0) : Constraint(
            {p1, p2, p3, p4}, 0.7, EQUALITY), _v0(v0) {};

    float evaluate() const override {
        glm::vec3 p1 = _particles[0]->predictedPosition;
        glm::vec3 p2 = _particles[1]->predictedPosition;
        glm::vec3 p3 = _particles[2]->predictedPosition;
        glm::vec3 p4 = _particles[3]->predictedPosition;

        glm::vec3 cross = glm::cross(p2 - p1, p3 - p1);

        return glm::dot(cross, (p4 - p1)) / 6.0f - _v0;
    }

private:
    float _v0;

    void computeGradient() override {
        //TODO: implement
    }
};

#endif //FEATHERGL_VOLUMECONSTRAINT_H
