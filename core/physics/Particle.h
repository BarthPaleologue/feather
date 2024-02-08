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
     * @param initialPosition The initial position of the particle in world space
     * @param positionIndex
     */
    Particle(float mass, glm::vec3 initialPosition, unsigned long positionIndex) {
        this->mass = mass;
        this->invMass = mass == 0 ? 0 : 1 / mass;
        this->initialPosition = initialPosition;
        position = initialPosition;
        this->positionIndex = positionIndex;
    }

    glm::vec3 resultingExternalForce() {
        glm::vec3 result = glm::vec3(0, 0, 0);
        for (auto force: externalForces) {
            result += force;
        }
        return result;
    }

    void reset() {
        position = initialPosition;
        velocity = glm::vec3(0, 0, 0);
    }

    float mass;
    float invMass;

    /**
     * Initial position of particle in world space
     */
    glm::vec3 initialPosition{};

    /**
     * Current position of particle in world space
     */
    glm::vec3 position{};

    /**
     * Predicted next position of particle in world space
     */
    glm::vec3 predictedPosition{};

    /**
     * Current velocity of particle
     */
    glm::vec3 velocity{};

    /**
     * The first index of the particle position in the vertex data array of the mesh
     */
    unsigned long positionIndex;

    std::vector<glm::vec3> externalForces{};
};

#endif //FEATHERGL_PARTICLE_H
