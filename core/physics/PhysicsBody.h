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
#include "IsometricBendConstraint.h"

class PhysicsBody {
public:
    PhysicsBody(std::shared_ptr<Mesh> mesh, float mass) : _mesh(mesh), _mass(mass) {

        _mesh->bakeRotationIntoVertexData();
        _mesh->bakeScalingIntoVertexData();
        auto meshPosition = *_mesh->transform()->position();

        unsigned int nbParticles = mesh->vertexData().positions.size() / 3;
        _particles.reserve(nbParticles);

        for (unsigned int i = 0; i < mesh->vertexData().positions.size(); i += 3) {
            auto particlePosition = glm::vec3(mesh->vertexData().positions[i], mesh->vertexData().positions[i + 1],
                                              mesh->vertexData().positions[i + 2]);
            _particles.push_back(
                    std::make_shared<Particle>(mass / (float) nbParticles, particlePosition + meshPosition, i));
        }
    };

    void buildParticleHierarchy(int nbLevels) {
        std::vector<std::vector<GLint>> triangulations;
        triangulations.push_back(mesh()->vertexData().indices);

        for (unsigned int i = 0; i < nbLevels; i++) {
            std::vector<GLint> coarseVertexIndices;
            std::vector<GLint> closestCoarseVertexIndices;
            std::vector<GLint> newTriangulation;

            std::vector<GLint> previousTriangulation = triangulations[triangulations.size() - 1];

            mesh()->vertexData().subset(previousTriangulation, coarseVertexIndices, closestCoarseVertexIndices,
                                        newTriangulation);

            triangulations.push_back(newTriangulation);
        }

        for (unsigned int i = 0; i < nbLevels; i++) {
            if(i == 0) {
                _nonCollisionConstraintsPerLevel.push_back(std::vector<Constraint *>(_nonCollisionConstraints));
            } else {
                _nonCollisionConstraintsPerLevel.push_back(std::vector<Constraint *>(_nonCollisionConstraintsPerLevel[i - 1]));
            }
        }
    }

    void updateVertexData() {
        for (const auto &particle: _particles) {
            particle->forces.clear();

            auto particleLocalPosition = particle->position - *transform()->position();

            // update actual mesh vertex data
            mesh()->vertexData().positions[particle->positionIndex] = particleLocalPosition.x;
            mesh()->vertexData().positions[particle->positionIndex + 1] = particleLocalPosition.y;
            mesh()->vertexData().positions[particle->positionIndex + 2] = particleLocalPosition.z;
        }

        mesh()->vertexData().computeNormals();
        mesh()->sendVertexDataToGPU();
    }

    std::vector<std::shared_ptr<Particle>> &particles() {
        return _particles;
    }

    std::shared_ptr<Mesh> mesh() {
        return _mesh;
    }

    Transform *transform() {
        return _mesh->transform();
    }

    void addFixedConstraint(FixedConstraint *constraint) {
        _nonCollisionConstraints.push_back(constraint);
        _fixedConstraints.push_back(constraint);
    }

    void addDistanceConstraint(DistanceConstraint *constraint) {
        _nonCollisionConstraints.push_back(constraint);
        _distanceConstraints.push_back(constraint);
    }

    void addBendConstraint(BendConstraint *constraint) {
        _nonCollisionConstraints.push_back(constraint);
        _bendConstraints.push_back(constraint);
    }

    void addIsometricBendConstraint(IsometricBendConstraint *constraint) {
        _nonCollisionConstraints.push_back(constraint);
        _isometricBendConstraints.push_back(constraint);
    }

    void addVolumeConstraint(VolumeConstraint *constraint) {
        _nonCollisionConstraints.push_back(constraint);
        _volumeConstraints.push_back(constraint);
    }

    void addCollisionConstraint(CollisionConstraint *constraint) {
        _collisionConstraints.push_back(constraint);
    }

    std::vector<Constraint *> &nonCollisionConstraints() {
        return _nonCollisionConstraints;
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
        for (const auto &particle: _particles) {
            particle->reset();
        }
    }

protected:
    float _mass;

    std::shared_ptr<Mesh> _mesh;
    std::vector<std::shared_ptr<Particle>> _particles;

private:
    std::vector<std::vector<Constraint *>> _nonCollisionConstraintsPerLevel;
    std::vector<Constraint *> _nonCollisionConstraints;

    std::vector<FixedConstraint *> _fixedConstraints;
    std::vector<DistanceConstraint *> _distanceConstraints;
    std::vector<BendConstraint *> _bendConstraints;
    std::vector<IsometricBendConstraint *> _isometricBendConstraints;
    std::vector<VolumeConstraint *> _volumeConstraints;
    std::vector<CollisionConstraint *> _collisionConstraints;
};

#endif //FEATHERGL_PHYSICSBODY_H
