//
// Created by barth on 19/09/2022.
//

#include "Scene.h"

void Scene::addDrawable(AbstractMesh &mesh) {
    _meshes.push_back(mesh);
}

void Scene::render(Camera &camera, PointLight &light) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (AbstractMesh &_drawable: _meshes) {
        _drawable.render(camera, light);
    }
}
