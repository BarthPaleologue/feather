//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_PARTICLE_H
#define FEATHERGL_PARTICLE_H

#include <glm/vec3.hpp>

struct Particle {
    Particle(float mass, float x0, float y0, float z0) {
        this->mass = mass;
        position = glm::vec3(x0, y0, z0);
        velocity = glm::vec3(0, 0, 0);
    }

    float mass;
    glm::vec3 position;
    glm::vec3 velocity;
};

#endif //FEATHERGL_PARTICLE_H
