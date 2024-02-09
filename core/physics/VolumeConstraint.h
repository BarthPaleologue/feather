//
// Created by barth on 15/11/23.
//

#ifndef FEATHERGL_VOLUMECONSTRAINT_H
#define FEATHERGL_VOLUMECONSTRAINT_H

#include <glm/geometric.hpp>
#include "Constraint.h"

class VolumeConstraint : public Constraint {
public:
    VolumeConstraint(std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2, std::shared_ptr<Particle> p3,
                     std::shared_ptr<Particle> p4, float restVolume, float compliance) : Constraint(
            {p1, p2, p3, p4}, compliance, EQUALITY), _restVolume(restVolume) {};

    float evaluate() const override {
        glm::vec3 p1 = _particles[0]->predictedPosition;
        glm::vec3 p2 = _particles[1]->predictedPosition;
        glm::vec3 p3 = _particles[2]->predictedPosition;
        glm::vec3 p4 = _particles[3]->predictedPosition;

        glm::vec3 cross = glm::cross(p2 - p1, p3 - p1) / 6.0f;

        return glm::dot(cross, (p4 - p1)) - _restVolume;
    }

private:
    float _restVolume;

    void computeGradient() override {
        glm::vec3 p0 = _particles[0]->predictedPosition;
        glm::vec3 p1 = _particles[1]->predictedPosition;
        glm::vec3 p2 = _particles[2]->predictedPosition;
        glm::vec3 p3 = _particles[3]->predictedPosition;

        glm::vec3 g1 = glm::cross(p2 - p0, p3 - p0) / 6.0f;
        glm::vec3 g2 = glm::cross(p3 - p0, p1 - p0) / 6.0f;
        glm::vec3 g3 = glm::cross(p1 - p0, p2 - p0) / 6.0f;
        glm::vec3 g0 = -g1 - g2 - g3;

        _gradient.col(0) = Eigen::Vector3f(g0.x, g0.y, g0.z);
        _gradient.col(1) = Eigen::Vector3f(g1.x, g1.y, g1.z);
        _gradient.col(2) = Eigen::Vector3f(g2.x, g2.y, g2.z);
        _gradient.col(3) = Eigen::Vector3f(g3.x, g3.y, g3.z);
    }

    void recomputeTargetValue() override {
        glm::vec3 p1 = _particles[0]->predictedPosition;
        glm::vec3 p2 = _particles[1]->predictedPosition;
        glm::vec3 p3 = _particles[2]->predictedPosition;
        glm::vec3 p4 = _particles[3]->predictedPosition;

        glm::vec3 cross = glm::cross(p2 - p1, p3 - p1) / 6.0f;

        _restVolume = glm::dot(cross, (p4 - p1));
    }
};

#endif //FEATHERGL_VOLUMECONSTRAINT_H
