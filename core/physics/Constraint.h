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
        _jacobian = Eigen::MatrixXf::Zero(3, _cardinality);
    }

    bool isSatisfied() const {
        switch (_type) {
            case UNILATERAL:
                return evaluate() >= 0;
            case BILATERAL:
                return evaluate() == 0;
        }
    }

protected:
    virtual float evaluate() const = 0;

    /// Number of particles involved in the constraint
    unsigned int _cardinality{};

    /// Particles involved in the constraint
    std::vector<Particle *> _particles;

    /// Stiffness of the constraint (between 0 and 1)
    float _stiffness{};

    ConstraintType _type;

    Eigen::MatrixXf _jacobian;
};

#endif //FEATHERGL_CONSTRAINT_H
