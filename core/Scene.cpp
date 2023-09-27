//
// Created by barth on 19/09/2022.
//

#include "Scene.h"

void Scene::addMesh(Mesh *mesh) {
    _meshes.push_back(mesh);
}

void Scene::render() {
    onBeforeRenderObservable.notifyObservers();

    _postProcesses[0]->StartProcessing();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (Mesh* _drawable: _meshes) {
        _drawable->render(_activeCamera, _pointLights);
    }

    for(int i = 0; i < _postProcesses.size() - 1; i++) {
        _postProcesses[i]->RenderTo(_postProcesses[i + 1]->getFBO());
    }

    _postProcesses[_postProcesses.size() - 1]->RenderToScreen();

    onAfterRenderObservable.notifyObservers();
}

void Scene::setActiveCamera(Camera *camera) {
    _activeCamera = camera;
}
