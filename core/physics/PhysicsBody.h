//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_PHYSICSBODY_H
#define FEATHERGL_PHYSICSBODY_H

#include "Mesh.h"
#include "Particle.h"
#include "Constraint.h"
#include "DistanceConstraint.h"
#include "FixedConstraint.h"
#include "CollisionConstraint.h"

class PhysicsBody {
public:
    PhysicsBody(std::shared_ptr<Mesh> mesh, float mass) : _mesh(mesh), _mass(mass) {
        _particles.reserve(mesh->vertexData().positions.size() / 3);
        unsigned int nbParticles = mesh->vertexData().positions.size() / 3;
        for (unsigned int i = 0; i < mesh->vertexData().positions.size(); i += 3) {
            _particles.push_back(new Particle(mass / nbParticles, mesh->vertexData().positions, i));
        }
    };

    void bakeTransformIntoVertexData() {
        auto worldMatrix = transform()->computeWorldMatrix();
        for (auto fixedConstraint: _fixedConstraints) {
            std::cout << fixedConstraint->targetPosition().x << ", " << fixedConstraint->targetPosition().y << ", "
                      << fixedConstraint->targetPosition().z << std::endl;
            fixedConstraint->setTargetPosition(worldMatrix * glm::vec4(fixedConstraint->targetPosition(), 1.0f));
            std::cout << fixedConstraint->targetPosition().x << ", " << fixedConstraint->targetPosition().y << ", "
                      << fixedConstraint->targetPosition().z << std::endl;
        }

        mesh()->bakeTransformIntoVertexData();

        for (unsigned int i = 0; i < _particles.size(); i++) {
            auto particlePosition = glm::vec3(_mesh->vertexData().positions[i * 3],
                                              _mesh->vertexData().positions[i * 3 + 1],
                                              _mesh->vertexData().positions[i * 3 + 2]);
            _particles[i]->position = particlePosition;
        }
    }

    void applyForcePerParticle(glm::vec3 force) {
        for (auto particle: _particles) {
            particle->forces.push_back(force);
        }
    }

    void buildParticleHierarchy() {
        //TODO: build particle hierarchy
    }

    std::vector<Particle *> &particles() {
        return _particles;
    }

    std::shared_ptr<Mesh> mesh() {
        return _mesh;
    }

    Transform *transform() {
        return _mesh->transform();
    }

    std::vector<Constraint *> &constraints() {
        return _constraints;
    }

    std::vector<FixedConstraint *> &fixedConstraints() {
        return _fixedConstraints;
    }

    std::vector<CollisionConstraint *> &collisionConstraints() {
        return _collisionConstraints;
    }

    float mass() const {
        return _mass;
    }

    unsigned int nbParticles() const {
        return _particles.size();
    }

protected:
    float _mass;

    std::shared_ptr<Mesh> _mesh;
    std::vector<Particle *> _particles;
    std::vector<Constraint *> _constraints;

    std::vector<FixedConstraint *> _fixedConstraints;
    std::vector<CollisionConstraint *> _collisionConstraints;
};

#endif //FEATHERGL_PHYSICSBODY_H
