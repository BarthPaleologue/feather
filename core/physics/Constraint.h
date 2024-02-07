//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_CONSTRAINT_H
#define FEATHERGL_CONSTRAINT_H

#include <functional>
#include <utility>
#include <memory>
#include "Particle.h"
#include "Core"

enum ConstraintType {
    INEQUALITY,
    EQUALITY
};

class Constraint {
public:
    Constraint(std::vector<std::shared_ptr<Particle>> particles, float stiffness, ConstraintType type) : _particles(
            std::move(particles)), _stiffness(stiffness), _type(type) {
        _cardinality = _particles.size();
        _gradient = Eigen::MatrixXf::Zero(3, _cardinality);
    }

    Constraint(const Constraint &other) : _particles(other._particles), _stiffness(other._stiffness), _type(other._type) {
        _cardinality = _particles.size();
        _gradient = Eigen::MatrixXf::Zero(3, _cardinality);
    }

    /**
     * Updates the predicted position of the particles according to the constraint
     */
    void solve() {
        // If the constraint is already satisfied, no need to solve it
        if (isSatisfied()) return;

        computeGradient();
        computeLambda();
        float correctedStiffness = (1 - powf(1 - _stiffness, 1.0f / 8.0f));

        for (unsigned int i = 0; i < _particles.size(); i++) {
            glm::vec3 gradient = glm::vec3(_gradient.col(i).x(), _gradient.col(i).y(), _gradient.col(i).z());
            _particles[i]->predictedPosition += _lambda * _particles[i]->invMass * gradient * correctedStiffness;
        }
    }

    std::vector<std::shared_ptr<Particle>> particles() const {
        return _particles;
    }

    void setParticles(std::vector<std::shared_ptr<Particle>> particles) {
        _particles = std::move(particles);
    }

    void setStiffness(float stiffness) {
        _stiffness = stiffness;
    }

    void replaceParticle(std::shared_ptr<Particle> oldParticle, std::shared_ptr<Particle> newParticle) {
        for (auto & _particle : _particles) {
            if (_particle != oldParticle) continue;

            _particle = newParticle;
        }
        recomputeTargetValue();
    }

    /**
     * Computes the constraint value
     * @return a float representing the constraint value
     */
    virtual float evaluate() const = 0;

protected:
    /**
     * Computes the gradient of the constraint
     */
    virtual void computeGradient() = 0;

    /**
     * Computes the s factor used to solve the constraint according to PBD paper
     */
    void computeLambda() {
        float numerator = -evaluate();
        float denominator = 0;
        if (_particles.size() != _gradient.cols()) {
            throw std::runtime_error("Gradient and particles size mismatch");
        }
        for (unsigned int i = 0; i < _particles.size(); i++) {
            denominator += _particles[i]->invMass * _gradient.col(i).dot(_gradient.col(i));
        }

        if (denominator < 1e-6) {
            _lambda = 0;
            return;
        }

        _lambda = numerator / denominator;

        if(std::isnan(_lambda)) {
            std::cout << "S is NAN" << std::endl;
            _lambda = 0.0;
        }
    }

    /**
     * Checks if the constraint is satisfied
     * @return true if the constraint is satisfied, false otherwise
     */
    bool isSatisfied() const {
        switch (_type) {
            case INEQUALITY:
                // >= 0
                return evaluate() >= 0;
            case EQUALITY:
                // == 0
                return fabsf(evaluate()) <= 1e-6;
        }

        throw std::runtime_error("Constraint type not handled");
    }

    virtual void recomputeTargetValue() = 0;

    /// Number of particles involved in the constraint
    unsigned int _cardinality{};

    /// Particles involved in the constraint
    std::vector<std::shared_ptr<Particle>> _particles;

    /// Stiffness of the constraint (between 0 and 1)
    float _stiffness{};

    /// s factor used to solve the constraint
    float _lambda{};

    /// Type of the constraint (equality or inequality)
    ConstraintType _type;

    /// Gradient of the constraint is a 3 x cardinality matrix
    Eigen::MatrixXf _gradient;
};

#endif //FEATHERGL_CONSTRAINT_H
