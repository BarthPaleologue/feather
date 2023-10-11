//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_PARTICLE_H
#define FEATHERGL_PARTICLE_H

#include <glm/vec3.hpp>
#include <vector>

struct Particle {
    Particle(float mass, std::vector<float> &positions, unsigned long startIndex) {
        this->mass = mass;
        this->invMass = 1 / mass;
        position = glm::vec3(positions[startIndex], positions[startIndex + 1], positions[startIndex + 2]);
        this->startIndex = startIndex;
    }

    float mass;
    float invMass;
    glm::vec3 position{};
    glm::vec3 predictedPosition{};
    glm::vec3 velocity{};
    unsigned long startIndex;
};

#endif //FEATHERGL_PARTICLE_H
