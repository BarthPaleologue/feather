//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_CLOTHBALLOONCONSTRAINT_H
#define FEATHERGL_CLOTHBALLOONCONSTRAINT_H

#include <glm/geometric.hpp>
#include "Constraint.h"

class ClothBalloonConstraint : public Constraint {
public:
    ClothBalloonConstraint(const std::vector<std::shared_ptr<Particle>> &particles, float pressure, float volume) : Constraint(
            particles, 1, EQUALITY) {
        _pressure = pressure;
        _volume = volume;
    }

private:
    float _pressure;
    float _volume;

    void computeGradient() override {
        //TODO: implement
    }

    float evaluate() const override {
        float volume = 0;
        for (int i = 0; i < _particles.size(); i++) {
            // FIXME: for i, we consider the 3 vertices in triangle i, so the following is wrong for now
            glm::vec3 p1 = _particles[i]->predictedPosition;
            glm::vec3 p2 = _particles[(i + 1) % _particles.size()]->predictedPosition;
            glm::vec3 p3 = _particles[(i + 2) % _particles.size()]->predictedPosition;

            volume += glm::dot(p1, glm::cross(p2, p3));
        }

        return volume - _volume * _pressure;
    }
};

#endif //FEATHERGL_CLOTHBALLOONCONSTRAINT_H
