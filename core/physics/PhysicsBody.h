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
#include "DihedralBendConstraint.h"
#include "GlobalVolumeConstraint.h"

class PhysicsBody {
public:
    PhysicsBody(std::shared_ptr<Mesh> mesh, float mass) : _mesh(mesh), _mass(mass) {
        _mesh->bakeRotationIntoVertexData();
        _mesh->bakeScalingIntoVertexData();
        auto meshPosition = _mesh->transform()->position();

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

        std::vector<std::vector<GLint>> closestCoarseVertexIndicesPerLevel;
        closestCoarseVertexIndicesPerLevel.push_back(std::vector<GLint>());
        for (int i = 0; i < _particles.size(); i++) {
            closestCoarseVertexIndicesPerLevel[0].push_back(i);
        }

        for (unsigned int level = 1; level <= nbLevels; level++) {
            std::vector<GLint> coarseVertexIndices;
            std::vector<GLint> closestCoarseVertexIndices;
            std::vector<GLint> newTriangulation;

            std::vector<GLint> previousTriangulation = triangulations[triangulations.size() - 1];

            mesh()->vertexData().subset(previousTriangulation, coarseVertexIndices, closestCoarseVertexIndices,
                                        newTriangulation);

            triangulations.push_back(newTriangulation);
            closestCoarseVertexIndicesPerLevel.push_back(closestCoarseVertexIndices);
        }

        for (unsigned int level = 0; level < nbLevels; level++) {
            if (level == 0) {
                _distanceConstraintsPerLevel.emplace_back(_distanceConstraints);
                continue;
            }

            _distanceConstraintsPerLevel.emplace_back(_distanceConstraintsPerLevel[level - 1]);

            std::vector<DistanceConstraint *> filteredConstraints{};

            // for each constraint, if a particle is not in the current level, change to constraint to use the closest particle in the current level
            for (auto &constraint: _distanceConstraintsPerLevel[level]) {
                bool shouldBeKept = true;

                auto constraintCopy = new DistanceConstraint(*constraint);

                for (auto &particle: constraintCopy->particles()) {
                    unsigned long particleIndex = particle->positionIndex / 3;

                    // check if the index is in the triangulation. If it is, the particle is in the current level
                    if (std::find(triangulations[level].begin(), triangulations[level].end(), particleIndex) !=
                        triangulations[level].end())
                        continue;

                    // the particle does not belong in this coarser level, we have to change it

                    // find the closest particle in the current level
                    GLint closestParticleIndex = closestCoarseVertexIndicesPerLevel[level][particleIndex];
                    if (closestParticleIndex == -1) {
                        shouldBeKept = false;
                        continue;
                    }

                    // replace the particle in the constraint
                    constraintCopy->replaceParticle(particle, _particles[closestParticleIndex]);
                }

                if (constraintCopy->particles()[0] == constraintCopy->particles()[1]) {
                    shouldBeKept = false;
                }

                if (shouldBeKept) {
                    filteredConstraints.push_back(constraintCopy);
                }
            }

            _distanceConstraintsPerLevel[level] = filteredConstraints;
        }

        // how many levels
        std::cout << "There are " << _distanceConstraintsPerLevel.size() << " levels" << std::endl;
        for (int i = 0; i < _distanceConstraintsPerLevel.size(); i++) {
            // how many constraints are there in this level
            std::cout << "Level " << i << " has " << _distanceConstraintsPerLevel[i].size() << " constraints"
                      << std::endl;
        }
    }

    void updateVertexData() {
        for (const auto &particle: _particles) {
            auto particleLocalPosition = particle->position - transform()->position();

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

    int nbParticles() {
        return (int) _particles.size();
    }

    void addFixedConstraint(FixedConstraint *constraint) {
        _fixedConstraints.push_back(constraint);
    }

    void addDistanceConstraint(DistanceConstraint *constraint) {
        _distanceConstraints.push_back(constraint);
    }

    void addBendConstraint(BendConstraint *constraint) {
        _bendConstraints.push_back(constraint);
    }

    void addDihedralBendConstraint(DihedralBendConstraint *constraint) {
        _dihedralBendConstraints.push_back(constraint);
    }

    void addVolumeConstraint(VolumeConstraint *constraint) {
        _volumeConstraints.push_back(constraint);
    }

    void addGlobalVolumeConstraint(GlobalVolumeConstraint *constraint) {
        _globalVolumeConstraints.push_back(constraint);
    }

    void addCollisionConstraint(CollisionConstraint *constraint) {
        _collisionConstraints.push_back(constraint);
    }

    std::vector<std::vector<DistanceConstraint *>> &distanceConstraintsPerLevel() {
        return _distanceConstraintsPerLevel;
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

    std::vector<GlobalVolumeConstraint *> &globalVolumeConstraints() {
        return _globalVolumeConstraints;
    }

    std::vector<CollisionConstraint *> &collisionConstraints() {
        return _collisionConstraints;
    }

    float mass() const {
        return _mass;
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
    std::vector<std::vector<DistanceConstraint *>> _distanceConstraintsPerLevel;

    std::vector<FixedConstraint *> _fixedConstraints;
    std::vector<DistanceConstraint *> _distanceConstraints;
    std::vector<BendConstraint *> _bendConstraints;
    std::vector<DihedralBendConstraint *> _dihedralBendConstraints;
    std::vector<VolumeConstraint *> _volumeConstraints;
    std::vector<GlobalVolumeConstraint *> _globalVolumeConstraints;
    std::vector<CollisionConstraint *> _collisionConstraints;
};

#endif //FEATHERGL_PHYSICSBODY_H
