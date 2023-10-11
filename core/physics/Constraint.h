//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_CONSTRAINT_H
#define FEATHERGL_CONSTRAINT_H

#include <functional>
#include <utility>
#include "Particle.h"
#include "Core"

enum ConstraintType {
    UNILATERAL,
    BILATERAL
};

class Constraint {
public:
    Constraint(std::vector<Particle *> particles, float stiffness, ConstraintType type) : _particles(
            std::move(particles)), _stiffness(stiffness), _type(type) {
        _cardinality = _particles.size();
        _gradient = Eigen::MatrixXf::Zero(3, _cardinality);
    }

    bool isSatisfied() const {
        switch (_type) {
            case UNILATERAL:
                // >= 0
                return evaluate() >= 1e-6;
            case BILATERAL:
                // == 0
                return fabsf(evaluate()) <= 1e-6;
        }
    }

    void solve() {
        if (isSatisfied()) return;

        computeGradient();
        computeLambda();
        for (unsigned int i = 0; i < _particles.size(); i++) {
            glm::vec3 gradient = glm::vec3(_gradient.col(i).x(), _gradient.col(i).y(), _gradient.col(i).z());
            _particles[i]->predictedPosition += -_lambda * _particles[i]->invMass * gradient;
        }
    }

protected:
    virtual void computeGradient() = 0;

    virtual float evaluate() const = 0;

    void computeLambda() {
        float numerator = -evaluate();
        float denominator = 0;
        for (unsigned int i = 0; i < _particles.size(); i++) {
            denominator += _particles[i]->invMass * _gradient.col(i).dot(_gradient.col(i));
        }

        _lambda = numerator / denominator;
    }

    /// Number of particles involved in the constraint
    unsigned int _cardinality{};

    /// Particles involved in the constraint
    std::vector<Particle *> _particles;

    /// Stiffness of the constraint (between 0 and 1)
    float _stiffness{};

    float _lambda{};

    ConstraintType _type;

    Eigen::MatrixXf _gradient;
};

#endif //FEATHERGL_CONSTRAINT_H
