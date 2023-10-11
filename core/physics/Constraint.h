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

    bool isSatisfied() const {
        switch (_type) {
            case UNILATERAL:
                return _constraintFunction(0, 0, 0) >= 0;
            case BILATERAL:
                return _constraintFunction(0, 0, 0) == 0;
        }
    }

private:
    int _cardinality;
    //FIXME: the signature of the function is incorrect (it should be float(3*cardinality float))
    std::function<float(float, float, float)> _constraintFunction;
    std::vector<int> _indices;

    /// Stiffness of the constraint (between 0 and 1)
    float _stiffness;

    ConstraintType _type;
};

#endif //FEATHERGL_CONSTRAINT_H
