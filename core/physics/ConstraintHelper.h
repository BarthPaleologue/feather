//
// Created by barth on 04/02/24.
//

#ifndef FEATHERGL_CONSTRAINTHELPER_H
#define FEATHERGL_CONSTRAINTHELPER_H

#include "MeshBuilder.h"
#include "Constraint.h"

class ConstraintHelper : public Mesh {
public:
    ConstraintHelper(Constraint *constraint, Scene &scene) : Mesh(
            *MeshBuilder::makeLine("ConstraintHelper", scene, constraint->particles()[0]->initialPosition,
                                  constraint->particles()[1]->initialPosition)), _constraint(constraint) {
        scene.onBeforeRenderObservable.add([this] {
            // update the line positions
            this->vertexData().positions[0] = _constraint->particles()[0]->position.x;
            this->vertexData().positions[1] = _constraint->particles()[0]->position.y;
            this->vertexData().positions[2] = _constraint->particles()[0]->position.z;

            this->vertexData().positions[3] = _constraint->particles()[1]->position.x;
            this->vertexData().positions[4] = _constraint->particles()[1]->position.y;
            this->vertexData().positions[5] = _constraint->particles()[1]->position.z;

            this->sendVertexDataToGPU();
        });
    }

private:
    Constraint *_constraint;
};

#endif //FEATHERGL_CONSTRAINTHELPER_H
