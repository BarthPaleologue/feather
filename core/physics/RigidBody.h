//
// Created by barth on 08/12/23.
//

#ifndef FEATHERGL_RIGIDBODY_H
#define FEATHERGL_RIGIDBODY_H

#include "PhysicsBody.h"
#include "MeshBuilder.h"
#include "VolumeConstraint.h"

class RigidBody: public PhysicsBody {
public:
    RigidBody(std::shared_ptr<Mesh> mesh, float mass) : PhysicsBody(mesh, mass) {
        // a rigidbody is a physics body where all particles are fixed relative to each other
        // iterate over all triangles and create distance constraints
        for(unsigned int i = 0; i < mesh->vertexData().indices.size(); i+= 3) {
            auto index1 = mesh->vertexData().indices[i];
            auto index2 = mesh->vertexData().indices[i + 1];
            auto index3 = mesh->vertexData().indices[i + 2];
            auto p1 = _particles[index1];
            auto p2 = _particles[index2];
            auto p3 = _particles[index3];
            addDistanceConstraint(std::make_shared<DistanceConstraint>(p1, p2, 0.0f));
            addDistanceConstraint(std::make_shared<DistanceConstraint>(p2, p3, 0.0f));
            addDistanceConstraint(std::make_shared<DistanceConstraint>(p3, p1, 0.0f));
        }

        // find particles that share their positions and create distance constraints
        for(unsigned int i = 0; i < _particles.size(); i++) {
            for(unsigned int j = i + 1; j < _particles.size(); j++) {
                if (glm::distance(_particles[i]->position, _particles[j]->position) < 0.001f) {
                    addDistanceConstraint(std::make_shared<DistanceConstraint>(_particles[i], _particles[j], 0.0f));
                }
            }
        }


        // bend constraints
        // create map of edge to list of triangles (triangles are represented by their first index in the indices array)
        std::map<std::pair<unsigned int, unsigned int>, std::vector<unsigned int>> edgeToTriangles;

        for (unsigned int i = 0; i < mesh->vertexData().indices.size(); i += 3) {
            auto index1 = mesh->vertexData().indices[i];
            auto index2 = mesh->vertexData().indices[i + 1];
            auto index3 = mesh->vertexData().indices[i + 2];

            auto edge1 = std::pair<unsigned int, unsigned int>(std::min(index1, index2), std::max(index1, index2));
            auto edge2 = std::pair<unsigned int, unsigned int>(std::min(index2, index3), std::max(index2, index3));
            auto edge3 = std::pair<unsigned int, unsigned int>(std::min(index3, index1), std::max(index3, index1));

            edgeToTriangles[edge1].push_back(i);
            edgeToTriangles[edge2].push_back(i);
            edgeToTriangles[edge3].push_back(i);
        }

        for (const auto& edgeToTrianglesPair: edgeToTriangles) {
            auto edge = edgeToTrianglesPair.first;
            auto triangles = edgeToTrianglesPair.second;

            if (triangles.size() != 2) continue;

            auto triangle1 = triangles[0];
            auto triangle2 = triangles[1];

            std::vector<unsigned int> notSharedVertices;
            for (unsigned int i = 0; i < 3; i++) {
                auto index = mesh->vertexData().indices[triangle1 + i];
                if (index != edge.first && index != edge.second) {
                    notSharedVertices.push_back(index);
                }

                index = mesh->vertexData().indices[triangle2 + i];
                if (index != edge.first && index != edge.second) {
                    notSharedVertices.push_back(index);
                }
            }

            if(notSharedVertices.size() != 2) continue;

            addBendConstraint(std::make_shared<FastBendConstraint>(_particles[edge.first], _particles[edge.second], _particles[notSharedVertices[0]], _particles[notSharedVertices[1]], 0.0f));
        }

        if(Utils::isMergedTriangulationClosed(mesh->vertexData().indices, mesh->vertexData().positions)) {
            // volume constraints
            addGlobalVolumeConstraint(std::make_shared<GlobalVolumeConstraint>(_particles, mesh->vertexData().indices, 1.0f, 0.0f));
        }
    }
};

#endif //FEATHERGL_RIGIDBODY_H
