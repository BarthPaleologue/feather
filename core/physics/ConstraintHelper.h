//
// Created by barth on 04/02/24.
//

#ifndef FEATHERGL_CONSTRAINTHELPER_H
#define FEATHERGL_CONSTRAINTHELPER_H

#include "MeshBuilder.h"
#include "Constraint.h"

class ConstraintHelper {
public:
    ConstraintHelper(Constraint *constraint, Scene &scene) : _mesh(
            MeshBuilder::makeLine("ConstraintHelper", scene, constraint->particles()[0]->initialPosition,
                                  constraint->particles()[1]->initialPosition)), _constraint(constraint) {
        scene.onBeforeRenderObservable.add([this] {
            if(!_mesh->isEnabled()) return;

            // update the line positions
            _mesh->vertexData().positions[0] = _constraint->particles()[0]->position.x;
            _mesh->vertexData().positions[1] = _constraint->particles()[0]->position.y;
            _mesh->vertexData().positions[2] = _constraint->particles()[0]->position.z;

            _mesh->vertexData().positions[3] = _constraint->particles()[1]->position.x;
            _mesh->vertexData().positions[4] = _constraint->particles()[1]->position.y;
            _mesh->vertexData().positions[5] = _constraint->particles()[1]->position.z;

            _mesh->sendVertexDataToGPU();
        });
    }

    std::shared_ptr<Mesh> mesh() {
        return _mesh;
    }

private:
    std::shared_ptr<Mesh> _mesh;
    Constraint *_constraint;
};

#endif //FEATHERGL_CONSTRAINTHELPER_H
