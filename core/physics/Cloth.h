//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_CLOTH_H
#define FEATHERGL_CLOTH_H

#include "PhysicsBody.h"
#include "MeshBuilder.h"
#include "DistanceConstraint.h"
#include "BendConstraint.h"

class Cloth : public PhysicsBody {
public:
    Cloth(const char *name, Scene &scene, unsigned int nbSubdivisions, float mass) : PhysicsBody(
            MeshBuilder::makePlane(name, scene, nbSubdivisions), mass) {
        // courtesy of myself https://github.com/BarthPaleologue/ClothSimulation/blob/master/src/ts/cloth.ts

        // horizontal stretch
        for (unsigned int x = 0; x < nbSubdivisions; x++) {
            for (unsigned int y = 0; y < nbSubdivisions - 1; y++) {
                auto particle1 = _particles[x * nbSubdivisions + y];
                auto particle2 = _particles[x * nbSubdivisions + y + 1];
                _constraints.push_back(new DistanceConstraint(particle1, particle2, 1.0f / (float) nbSubdivisions));
            }
        }

        // vertical stretch
        for (unsigned int x = 0; x < nbSubdivisions - 1; x++) {
            for (unsigned int y = 0; y < nbSubdivisions; y++) {
                auto particle1 = _particles[x * nbSubdivisions + y];
                auto particle2 = _particles[(x + 1) * nbSubdivisions + y];
                _constraints.push_back(new DistanceConstraint(particle1, particle2, 1.0f / (float) nbSubdivisions));
            }
        }

        // bend
        /*for(unsigned int x = 0; x < nbSubdivisions - 2; x++) {
            for(unsigned int y = 0; y < nbSubdivisions - 2; y++) {
                auto particle1 = _particles[x * nbSubdivisions + y];
                auto particle2 = _particles[x * nbSubdivisions + y + 2];
                auto particle3 = _particles[(x + 2) * nbSubdivisions + y];
                auto particle4 = _particles[(x + 2) * nbSubdivisions + y + 2];
                _constraints.push_back(new BendConstraint(particle1, particle2, particle3, particle4, 0.0f));
            }
        }*/
    };
};

#endif //FEATHERGL_CLOTH_H
