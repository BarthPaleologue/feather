//
// Created by barth on 03/02/24.
//

#ifndef FEATHERGL_DIHEDRALBENDCONSTRAINT_H
#define FEATHERGL_DIHEDRALBENDCONSTRAINT_H

#include <glm/geometric.hpp>
#include <iostream>
#include "Constraint.h"
#include "utils.h"

class DihedralBendConstraint: public Constraint {
public:
    DihedralBendConstraint(std::shared_ptr<Particle> p0, std::shared_ptr<Particle> p1, std::shared_ptr<Particle> p2, std::shared_ptr<Particle> p3, float compliance) : Constraint(
    {p0, p1, p2, p3}, compliance, EQUALITY) {

            glm::vec3 p_0 = _particles[0]->position;
            glm::vec3 p_1 = _particles[1]->position;
            glm::vec3 p_2 = _particles[2]->position;
            glm::vec3 p_3 = _particles[3]->position;

            glm::vec3 n1 = glm::normalize(glm::cross(p_1 - p_0, p_2 - p_0));
            glm::vec3 n2 = glm::normalize(glm::cross(p_1 - p_0, p_3 - p_0));

            float dot = glm::dot(n1, n2);

            // clamp dot product to -1..1 because of floating point precision
            if (dot > 1.0f) dot = 1.0f;
            if (dot < -1.0f) dot = -1.0f;

            _phi = acosf(dot);
            std::cout << "PHI: " << _phi << std::endl;
    };


    float evaluate() const override {
        glm::vec3 p0 = _particles[0]->predictedPosition;
        glm::vec3 p1 = _particles[1]->predictedPosition;
        glm::vec3 p2 = _particles[2]->predictedPosition;
        glm::vec3 p3 = _particles[3]->predictedPosition;

        //std::cout << "POSITIONS: " << toString(p0) << toString(p1) << toString(p2) << toString(p3) << std::endl;

        glm::vec3 sharedEdgeDir = glm::normalize(p1 - p0);

        glm::vec3 n1 = glm::normalize(glm::cross(p1 - p0, p2 - p0));
        glm::vec3 n2 = glm::normalize(glm::cross(p1 - p0, p3 - p0));

        float dot = glm::dot(n1, n2);

        // clamp dot product to -1..1 because of floating point precision
        if (dot > 1.0f) dot = 1.0f;
        if (dot < -1.0f) dot = -1.0f;

        float phi = acosf(dot);
        if(glm::dot(glm::cross(n1, n2), sharedEdgeDir) < 0.0f) phi = 2.0f * M_PIf - phi;

        //std::cout << "PHI: " << phi - _phi << std::endl;

        return phi - _phi;
    }

private:
    float _phi;

    void computeGradient() override {
        glm::vec3 p0 = _particles[0]->predictedPosition;
        glm::vec3 p1 = _particles[1]->predictedPosition;
        glm::vec3 p2 = _particles[2]->predictedPosition;
        glm::vec3 p3 = _particles[3]->predictedPosition;

        glm::vec3 el = p2 - p0;
        glm::vec3 em = p1 - p0;
        glm::vec3 er = p3 - p0;

        // Compute n1 = p1 x p2, n2 = p1 x p3 and cosPhi = n1.n2
        glm::vec3 n1 = glm::cross(p1 - p0, p2 - p0);
        glm::vec3 n2 = glm::cross(p1 - p0, p3 - p0);

        float l1 = glm::length(n1);
        float l2 = glm::length(n2);

        // Check if n1 or n2 are null vectors (i.e. the triangle is degenerate)
        if (l1 == 0.0f || l2 == 0.0f) return;

        // Normalize n1 and n2
        n1 /= l1;
        n2 /= l2;

        float cosPhi = glm::dot(n1, n2);
        if (cosPhi > 1.0f) cosPhi = 1.0f;
        if (cosPhi < -1.0f) cosPhi = -1.0f;

        float cosPhi2 = cosPhi * cosPhi;

        if (cosPhi2 == 1.0f) return;

        float arcosDerivative = -1.0f / sqrtf(1.0f - cosPhi2);
        if(glm::dot(glm::cross(n1, n2), em) < 0.0f) arcosDerivative = -arcosDerivative;

        glm::vec3 dp1 = (glm::cross(er, n1) + cosPhi * glm::cross(n2, er)) / l2
                        + (glm::cross(el, n2) + cosPhi * glm::cross(n1, el)) / l1;
        glm::vec3 dp2 = (glm::cross(n2, em) - cosPhi * glm::cross(n1, em)) / l1;
        glm::vec3 dp3 = (glm::cross(n1, em) - cosPhi * glm::cross(n2, em)) / l2;

        //std::cout << "DERIVATIVES: " << toString(dp1) << toString(dp2) << toString(dp3) << std::endl;

        Eigen::Vector3f grad1 = arcosDerivative * Eigen::Vector3f(dp1.x, dp1.y, dp1.z);
        Eigen::Vector3f grad2 = arcosDerivative * Eigen::Vector3f(dp2.x, dp2.y, dp2.z);
        Eigen::Vector3f grad3 = arcosDerivative * Eigen::Vector3f(dp3.x, dp3.y, dp3.z);
        Eigen::Vector3f grad0 = -grad1 - grad2 - grad3;

        _gradient.col(0) = grad0;
        _gradient.col(1) = grad1;
        _gradient.col(2) = grad2;
        _gradient.col(3) = grad3;
    }

    void recomputeTargetValue() override {
        glm::vec3 p_0 = _particles[0]->position;
        glm::vec3 p_1 = _particles[1]->position;
        glm::vec3 p_2 = _particles[2]->position;
        glm::vec3 p_3 = _particles[3]->position;

        glm::vec3 n1 = glm::normalize(glm::cross(p_1 - p_0, p_2 - p_0));
        glm::vec3 n2 = glm::normalize(glm::cross(p_1 - p_0, p_3 - p_0));

        float dot = glm::dot(n1, n2);

        // clamp dot product to -1..1 because of floating point precision
        if (dot > 1.0f) dot = 1.0f;
        if (dot < -1.0f) dot = -1.0f;

        _phi = acosf(dot);
    }
};


#endif //FEATHERGL_DIHEDRALBENDCONSTRAINT_H
