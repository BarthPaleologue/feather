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
    CollisionConstraint(std::shared_ptr<Particle> q, std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2,
                        std::shared_ptr<Particle> p3) : Constraint(
            {q, p1, p2, p3}, 1.0f, INEQUALITY), _h(0.01f) {};


    float evaluate() const override {
        glm::vec3 oldQ = _particles[0]->position;
        glm::vec3 q = _particles[0]->predictedPosition;
        glm::vec3 p1 = _particles[1]->predictedPosition;
        glm::vec3 p2 = _particles[2]->predictedPosition;
        glm::vec3 p3 = _particles[3]->predictedPosition;

        glm::vec3 n = glm::cross(p2 - p1, p3 - p1);
        // if triangle is degenerate, normalization would result in NaN values
        if (glm::length(n) < 1e-3f) {
            return 0.0f;
        }
        n = glm::normalize(n);

        return glm::dot(q - p1, n) - _h;
    }

private:
    float _h;

    void computeGradient() override {
        glm::vec3 oldQ = _particles[0]->position;
        glm::vec3 q = _particles[0]->predictedPosition;
        glm::vec3 p1 = _particles[1]->predictedPosition;
        glm::vec3 p2 = _particles[2]->predictedPosition;
        glm::vec3 p3 = _particles[3]->predictedPosition;

        Eigen::Vector3f p21 = Eigen::Vector3f(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
        Eigen::Vector3f p31 = Eigen::Vector3f(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);

        glm::vec3 n = glm::cross(p2 - p1, p3 - p1);
        if (glm::length(n) < 1e-3f) {
            _gradient = Eigen::MatrixXf::Zero(3, 4);
            return;
        }
        n = glm::normalize(n);
        //std::cout << toString(n) << std::endl;

        //n *= glm::length(_particles[0]->velocity) * 0.2f;

        Eigen::MatrixXf gradP2 = Utils::crossProdGrad_p1(p21, p31);
        Eigen::MatrixXf gradP3 = Utils::crossProdGrad_p2(p21, p31);

        Eigen::Vector3f _qEigen = Eigen::Vector3f(q.x - p1.x, q.y - p1.y, q.z - p1.z);

        _gradient.col(0) = Eigen::Vector3f(n.x, n.y, n.z);
        _gradient.col(1) = gradP2 * _qEigen;
        _gradient.col(2) = gradP3 * _qEigen;
        _gradient.col(3) = -_gradient.col(0) - _gradient.col(1) - _gradient.col(2);
    }

    void recomputeTargetValue() override {
        _h = 0.01f;
    }
};

#endif //FEATHERGL_COLLISIONCONSTRAINT_H
