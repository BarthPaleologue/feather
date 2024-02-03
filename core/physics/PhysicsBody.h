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
            if (i == 0) {
                _nonCollisionConstraintsPerLevel.emplace_back(_nonCollisionConstraints);
            } else {
                _nonCollisionConstraintsPerLevel.emplace_back(_nonCollisionConstraintsPerLevel[i - 1]);
            }

            // for each constraint, if a particle is not in the current level, change to constraint to use the closest particle in the current level
            for (auto &constraint: _nonCollisionConstraintsPerLevel[i]) {
                for (auto &particle: constraint->particles()) {
                    unsigned long particleIndex = particle->positionIndex / 3;

                    // check if the index is in the triangulation. If it is, the particle is in the current level
                    if (std::find(triangulations[i].begin(), triangulations[i].end(), particleIndex) !=
                        triangulations[i].end())
                        continue;

                    // the particle does not belong in this coarser level, we have to change it

                    // find the closest particle in the current level
                    float minDistance = std::numeric_limits<float>::max();
                    std::shared_ptr<Particle> closestParticle = nullptr;
                    for (auto &currentParticle: _particles) {
                        if (std::find(triangulations[i].begin(), triangulations[i].end(),
                                      currentParticle->positionIndex / 3) != triangulations[i].end()) {
                            float distance = glm::distance(particle->position, currentParticle->position);
                            if (distance < minDistance) {
                                minDistance = distance;
                                closestParticle = currentParticle;
                            }
                        }
                    }

                    // replace the particle in the constraint
                    constraint->replaceParticle(particle, closestParticle);
                }
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
