//
// Created by barth on 09/10/23.
//

#ifndef FEATHERGL_MESHLOADER_H
#define FEATHERGL_MESHLOADER_H

#include "Mesh.h"
#include "Scene.h"
#include "MeshBuilder.h"

class MeshLoader {
public:
    static Mesh *loadObj(const char *path, Scene &scene) {
        VertexData vertexData = VertexData();

        std::string s;
        std::ifstream fin(path);
        if (!fin) throw std::runtime_error("File not found: " + std::string(path));
        int i = 0;
        while (fin >> s) {
            i++;
            std::cout << s << std::endl;
            switch (*s.c_str()) {
                case 'v': {
                    float x, y, z;
                    fin >> x >> y >> z;
                    vertexData.positions.push_back(x);
                    vertexData.positions.push_back(y);
                    vertexData.positions.push_back(z);
                    break;
                }
                case 'f': {
                    int v1, v2, v3;
                    vertexData.indices.push_back(v1);
                    vertexData.indices.push_back(v2);
                    vertexData.indices.push_back(v3);
                    break;
                }
                default:
                    break;
            }
        }

        std::cout << i << std::endl;

        auto mesh = Mesh::FromVertexData("obj", vertexData);
        scene.addMesh(mesh);
        return mesh;
    }
};

#endif //FEATHERGL_MESHLOADER_H
