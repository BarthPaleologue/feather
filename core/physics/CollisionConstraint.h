//
// Created by barth on 15/11/23.
//

#ifndef FEATHERGL_COLLISIONCONSTRAINT_H
#define FEATHERGL_COLLISIONCONSTRAINT_H

#include <glm/geometric.hpp>
#include "Particle.h"
#include "Constraint.h"

class CollisionConstraint : public Constraint {
public:
    CollisionConstraint(std::shared_ptr<Particle> q, std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2, std::shared_ptr<Particle> p3, float h) : Constraint(
            {q, p1, p2, p3}, 1.0f, INEQUALITY), _h(h) {};

private:
    float _h;

    float evaluate() const override {
        glm::vec3 q = _particles[0]->predictedPosition;
        glm::vec3 p1 = _particles[1]->predictedPosition;
        glm::vec3 p2 = _particles[2]->predictedPosition;
        glm::vec3 p3 = _particles[3]->predictedPosition;

        glm::vec3 n = glm::normalize(glm::cross(p2 - p1, p3 - p1));

        return glm::dot(q - p1, n) - _h;
    }

    void computeGradient() override {
        glm::vec3 p1 = _particles[1]->predictedPosition;
        glm::vec3 p2 = _particles[2]->predictedPosition;
        glm::vec3 p3 = _particles[3]->predictedPosition;

        glm::vec3 n = glm::normalize(glm::cross(p2 - p1, p3 - p1));

        _gradient.col(0) = Eigen::Vector3f(n.x, n.y, n.z);
        _gradient.col(1) = Eigen::Vector3f(-n.x, -n.y, -n.z);
        _gradient.col(2) = Eigen::Vector3f(0, 0, 0);
        _gradient.col(3) = Eigen::Vector3f(0, 0, 0);
    }
};

#endif //FEATHERGL_COLLISIONCONSTRAINT_H
