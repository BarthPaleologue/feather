//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_CONSTRAINT_H
#define FEATHERGL_CONSTRAINT_H

#include <functional>
#include "Particle.h"

enum ConstraintType {
    UNILATERAL,
    BILATERAL
};

class Constraint {
public:
    int cardinality() const {
        return _cardinality;
    }

    float stiffness() const {
        return _stiffness;
    }

    ConstraintType type() const {
        return _type;
    }

    bool isSatisfied() const {
        switch (_type) {
            case UNILATERAL:
                return evaluate() >= 0;
            case BILATERAL:
                return evaluate() == 0;
        }
    }

private:
    virtual float evaluate() const = 0;

    int _cardinality;
    
    std::vector<Particle *> _particles;

    /// Stiffness of the constraint (between 0 and 1)
    float _stiffness;

    ConstraintType _type;
};

#endif //FEATHERGL_CONSTRAINT_H
