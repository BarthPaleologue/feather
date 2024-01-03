//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_CLOTH_H
#define FEATHERGL_CLOTH_H

#include "PhysicsBody.h"
#include "MeshBuilder.h"
#include "DistanceConstraint.h"
#include "BendConstraint.h"
#include "FixedConstraint.h"

class Cloth : public PhysicsBody {
public:
    Cloth(const char *name, Scene &scene, unsigned int nbSubdivisions, float mass) : PhysicsBody(
            MeshBuilder::makePlane(name, scene, nbSubdivisions), mass) {
        // courtesy of myself https://github.com/BarthPaleologue/ClothSimulation/blob/master/src/ts/cloth.ts

        // Seed the random number generator
        std::random_device rd;
        std::mt19937 gen(rd());

        // Define the distribution for indices
        std::uniform_int_distribution<> distribution(0, _particles.size() / 2);

        // Generate a random index
        int randomIndex1 = distribution(gen);
        int randomIndex2 = distribution(gen);

        // fixed particles
        auto topLeft = new FixedConstraint(_particles[randomIndex1], _particles[randomIndex1]->position);
        addFixedConstraint(topLeft);

        auto topRight = new FixedConstraint(_particles[randomIndex2], _particles[randomIndex2]->position);
        addFixedConstraint(topRight);

        float stiffness = 0.5f;

        // horizontal stretch
        for (unsigned int x = 0; x < nbSubdivisions; x++) {
            for (unsigned int y = 0; y < nbSubdivisions - 1; y++) {
                auto particle1 = _particles[x * nbSubdivisions + y];
                auto particle2 = _particles[x * nbSubdivisions + y + 1];
                auto distanceConstraint = new DistanceConstraint(particle1, particle2, 1.0f / (float) nbSubdivisions,
                                                                 stiffness);
                addDistanceConstraint(distanceConstraint);
            }
        }

        // vertical stretch
        for (unsigned int x = 0; x < nbSubdivisions - 1; x++) {
            for (unsigned int y = 0; y < nbSubdivisions; y++) {
                auto particle1 = _particles[x * nbSubdivisions + y];
                auto particle2 = _particles[(x + 1) * nbSubdivisions + y];
                auto distanceConstraint = new DistanceConstraint(particle1, particle2, 1.0f / (float) nbSubdivisions,
                                                                 stiffness);
                addDistanceConstraint(distanceConstraint);
            }
        }

        // bend
        for (unsigned int x = 0; x < nbSubdivisions - 1; x++) {
            for (unsigned int y = 0; y < nbSubdivisions - 1; y++) {
                auto particle1 = _particles[x * nbSubdivisions + y];
                auto particle2 = _particles[x * nbSubdivisions + y + 1];
                auto particle3 = _particles[(x + 1) * nbSubdivisions + y];
                auto particle4 = _particles[(x + 1) * nbSubdivisions + y + 1];
                //addBendConstraint(new BendConstraint(particle2, particle3, particle1, particle4, 0.01f));
                std::cout << 1.414f / (float) nbSubdivisions << ";" << glm::length(particle1->position - particle4->position) << std::endl;
                addBendConstraint(new BendConstraint(particle2, particle3, particle1, particle4, 0.5f));
            }
        }
    };
};

#endif //FEATHERGL_CLOTH_H
