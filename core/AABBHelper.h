//
// Created by barth on 04/02/24.
//

#ifndef FEATHERGL_AABBHELPER_H
#define FEATHERGL_AABBHELPER_H

#include <glm/glm.hpp>
#include "AABB.h"
#include "MeshBuilder.h"

class AABBHelper {
public:
    AABBHelper(AABB* aabb, Scene &scene): _mesh(MeshBuilder::makeCube("aabb", scene)), _aabb(aabb) {
        _mesh->material()->setWireframe(true);
        scene.onBeforeRenderObservable.add([&] {
            _mesh->transform()->setScaling(_aabb->size() * 0.5f);
            _mesh->transform()->setPosition(_aabb->center());
        });
    }

private:
    AABB *_aabb;
    std::shared_ptr<Mesh> _mesh;
};

#endif //FEATHERGL_AABBHELPER_H
