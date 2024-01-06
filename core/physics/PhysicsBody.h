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
#include "BendConstraint.h"
#include "VolumeConstraint.h"

class PhysicsBody {
public:
    PhysicsBody(std::shared_ptr<Mesh> mesh, float mass) : _mesh(mesh), _mass(mass) {
        _particles.reserve(mesh->vertexData().positions.size() / 3);
        unsigned int nbParticles = mesh->vertexData().positions.size() / 3;
        auto worldMatrix = mesh->transform()->computeWorldMatrix();
        for (unsigned int i = 0; i < mesh->vertexData().positions.size(); i += 3) {
            auto particlePosition4 = worldMatrix * glm::vec4(mesh->vertexData().positions[i], mesh->vertexData().positions[i + 1], mesh->vertexData().positions[i + 2], 1.0f);
            auto particlePosition = glm::vec3(particlePosition4.x, particlePosition4.y, particlePosition4.z);
            _particles.push_back(new Particle(mass / (float)nbParticles, particlePosition, i));
        }
    };

    void buildParticleHierarchy(int nbLevels) {
        std::vector<std::vector<GLint>> triangulations;
        triangulations.push_back(mesh()->vertexData().indices);

        for(unsigned int i = 0; i < nbLevels; i++) {
            std::vector<GLint> coarseVertexIndices;
            std::vector<GLint> closestCoarseVertexIndices;
            std::vector<GLint> newTriangulation;

            std::vector<GLint> previousTriangulation = triangulations[triangulations.size() - 1];

            mesh()->vertexData().subset(previousTriangulation, coarseVertexIndices, closestCoarseVertexIndices, newTriangulation);

            triangulations.push_back(newTriangulation);
        }
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

    void addFixedConstraint(FixedConstraint *constraint) {
        _constraints.push_back(constraint);
        _fixedConstraints.push_back(constraint);
    }

    void addDistanceConstraint(DistanceConstraint *constraint) {
        _constraints.push_back(constraint);
        _distanceConstraints.push_back(constraint);
    }

    void addBendConstraint(BendConstraint *constraint) {
        _constraints.push_back(constraint);
        _bendConstraints.push_back(constraint);
    }

    void addVolumeConstraint(VolumeConstraint *constraint) {
        _constraints.push_back(constraint);
        _volumeConstraints.push_back(constraint);
    }

    void addCollisionConstraint(CollisionConstraint *constraint) {
        _constraints.push_back(constraint);
        _collisionConstraints.push_back(constraint);
    }

    std::vector<Constraint *> &constraints() {
        return _constraints;
    }

    std::vector<FixedConstraint *> &fixedConstraints() {
        return _fixedConstraints;
    }

    std::vector<DistanceConstraint *> &distanceConstraints() {
        return _distanceConstraints;
    }

    std::vector<BendConstraint *> &bendConstraints() {
        return _bendConstraints;
    }

    std::vector<VolumeConstraint *> &volumeConstraints() {
        return _volumeConstraints;
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

    void reset() {
        for (auto particle: _particles) {
            particle->reset();
        }
    }

protected:
    float _mass;

    std::shared_ptr<Mesh> _mesh;
    std::vector<Particle *> _particles;

private:
    std::vector<Constraint *> _constraints;
    std::vector<FixedConstraint *> _fixedConstraints;
    std::vector<DistanceConstraint *> _distanceConstraints;
    std::vector<BendConstraint *> _bendConstraints;
    std::vector<VolumeConstraint *> _volumeConstraints;
    std::vector<CollisionConstraint *> _collisionConstraints;
};

#endif //FEATHERGL_PHYSICSBODY_H
