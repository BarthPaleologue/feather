//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_CLOTH_H
#define FEATHERGL_CLOTH_H

#include "PhysicsBody.h"
#include "MeshBuilder.h"
#include "StretchConstraint.h"
#include "BendConstraint.h"

class Cloth : public PhysicsBody {
public:
    Cloth(const char *name, Scene &scene, unsigned int nbSubdivisions, float mass) : PhysicsBody(
            MeshBuilder::makePlane(name, scene, nbSubdivisions), mass) {
        for (int i = 0; i < _particles.size(); i++) {
            // vertical stretch
            if (i < _particles.size() - 1) {
                _constraints.push_back(new StretchConstraint(_particles[i], _particles[i + 1], 1.0));
            }

            // horizontal stretch
            if (i < _particles.size() - nbSubdivisions) {
                _constraints.push_back(new StretchConstraint(_particles[i], _particles[i + nbSubdivisions], 1.0));
            }

            // bending
            if (i < _particles.size() - nbSubdivisions - 1) {
                _constraints.push_back(new BendConstraint(_particles[i], _particles[i + nbSubdivisions + 1],
                                                          _particles[i + nbSubdivisions], _particles[i + 1], 1.0));
            }
        }
    };
};

#endif //FEATHERGL_CLOTH_H
