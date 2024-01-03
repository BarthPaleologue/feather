//
// Created by barth on 08/12/23.
//

#ifndef FEATHERGL_SOFTBODY_H
#define FEATHERGL_SOFTBODY_H

#include "PhysicsBody.h"

#include <map>
#include <utility>

class SoftBody : public PhysicsBody {
public:
    SoftBody(std::shared_ptr<Mesh> mesh, float mass) : PhysicsBody(mesh, mass) {
        float stiffness = 0.5f;

        // iterate over all triangles and create distance constraints
        for (unsigned int i = 0; i < mesh->vertexData().indices.size(); i += 3) {
            auto index1 = mesh->vertexData().indices[i];
            auto index2 = mesh->vertexData().indices[i + 1];
            auto index3 = mesh->vertexData().indices[i + 2];
            auto p1 = _particles[index1];
            auto p2 = _particles[index2];
            auto p3 = _particles[index3];
            addDistanceConstraint(new DistanceConstraint(p1, p2, glm::length(p1->position - p2->position), stiffness));
            addDistanceConstraint(new DistanceConstraint(p2, p3, glm::length(p2->position - p3->position), stiffness));
            addDistanceConstraint(new DistanceConstraint(p3, p1, glm::length(p3->position - p1->position), stiffness));
        }

        // find particles that share their positions and create distance constraints
        for (unsigned int i = 0; i < _particles.size(); i++) {
            for (unsigned int j = i + 1; j < _particles.size(); j++) {
                if (_particles[i]->position == _particles[j]->position) {
                    addDistanceConstraint(new DistanceConstraint(_particles[i], _particles[j], 0.0f, 1.0f));
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

            std::cout << "EDGE " << edge1.first << ", " << edge1.second << std::endl;
            std::cout << "EDGE " << edge2.first << ", " << edge2.second << std::endl;
            std::cout << "EDGE " << edge3.first << ", " << edge3.second << std::endl;
            std::cout << "==============" << std::endl;

            edgeToTriangles[edge1].push_back(i);
            edgeToTriangles[edge2].push_back(i);
            edgeToTriangles[edge3].push_back(i);
        }

        for (const auto& edgeToTrianglesPair: edgeToTriangles) {
            auto edge = edgeToTrianglesPair.first;
            auto triangles = edgeToTrianglesPair.second;

            std::cout << triangles.size() << " TRIANGLES" << std::endl;

            if (triangles.size() != 2) continue;

            std::cout << "EDGE " << edge.first << ", " << edge.second << " is shared by two triangles: " << std::endl;
            std::cout << mesh->vertexData().indices[triangles[0]] << ", " << mesh->vertexData().indices[triangles[0] + 1] << ", " << mesh->vertexData().indices[triangles[0] + 2] << std::endl;
            std::cout << mesh->vertexData().indices[triangles[1]] << ", " << mesh->vertexData().indices[triangles[1] + 1] << ", " << mesh->vertexData().indices[triangles[1] + 2] << std::endl;


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

            addBendConstraint(new BendConstraint(_particles[edge.first], _particles[edge.second], _particles[notSharedVertices[0]], _particles[notSharedVertices[1]], 0.7f));
        }
    }
};

#endif //FEATHERGL_SOFTBODY_H
