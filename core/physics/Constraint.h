//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_CONSTRAINT_H
#define FEATHERGL_CONSTRAINT_H

#include <functional>

enum ConstraintType {
    UNILATERAL,
    BILATERAL
};

class Constraint {
public:
    int cardinality() const {
        return _cardinality;
    }

    std::function<float(float, float, float)> constraintFunction() const {
        return _constraintFunction;
    }

    const std::vector<int> *const indices() const {
        return &_indices;
    }

    float stiffness() const {
        return _stiffness;
    }

    ConstraintType type() const {
        return _type;
    }

private:
    int _cardinality;
    std::function<float(float, float, float)> _constraintFunction;
    std::vector<int> _indices;
    float _stiffness;
    ConstraintType _type;
};

#endif //FEATHERGL_CONSTRAINT_H
