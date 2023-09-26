//
// Created by barth on 19/09/2022.
//

#include "Scene.h"

void Scene::addMesh(Mesh *mesh) {
    _meshes.push_back(mesh);
}

void Scene::render(PointLight &light) {
    onBeforeRenderObservable.notifyObservers();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (Mesh* _drawable: _meshes) {
        _drawable->render(*_activeCamera, light);
    }

    onAfterRenderObservable.notifyObservers();
}

void Scene::setActiveCamera(Camera *camera) {
    _activeCamera = camera;
}
