//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_PARTICLE_H
#define FEATHERGL_PARTICLE_H

#include <glm/vec3.hpp>
#include <vector>

struct Particle {
    /**
     * @brief Construct a new Particle object
     * @param mass The mass of the particle. (0 for static particles)
     * @param positions
     * @param startIndex
     */
    Particle(float mass, std::vector<float> &positions, unsigned long startIndex) {
        this->mass = mass;
        this->invMass = mass == 0 ? 0 : 1 / mass;
        position = glm::vec3(positions[startIndex], positions[startIndex + 1], positions[startIndex + 2]);
        this->startIndex = startIndex;
    }

    glm::vec3 resultingForce() {
        glm::vec3 result = glm::vec3(0, 0, 0);
        for (auto force: forces) {
            result += force;
        }
        return result;
    }

    float mass;
    float invMass;
    glm::vec3 position{};
    glm::vec3 predictedPosition{};
    glm::vec3 velocity{};
    unsigned long startIndex;

    std::vector<glm::vec3> forces{};
};

#endif //FEATHERGL_PARTICLE_H
