//
// Created by barth on 19/09/2022.
//

#include "Scene.h"

void Scene::addMesh(std::shared_ptr<Mesh> mesh) {
    _meshes.push_back(mesh);
}

void Scene::render() {
    onBeforeRenderObservable.notifyObservers();

    // Shadow pass
    for (auto shadowRenderer: _shadowRenderers) {
        shadowRenderer->bind();
        for (auto _drawable: _meshes) {
            _drawable->render(_activeCamera->getProjectionViewMatrix());
        }
        shadowRenderer->unbind();
    }

    if (!_postProcesses.empty()) {
        glBindFramebuffer(GL_FRAMEBUFFER, _postProcesses[0]->getFBO());
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (auto _drawable: _meshes) {
        _drawable->render(_activeCamera->getProjectionViewMatrix());
    }

    if (!_postProcesses.empty()) {
        for (int i = 0; i < _postProcesses.size() - 1; i++) {
            _postProcesses[i]->RenderTo(_postProcesses[i + 1]->getFBO());
        }
        _postProcesses[_postProcesses.size() - 1]->RenderToScreen();
    }

    onAfterRenderObservable.notifyObservers();
}

void Scene::setActiveCamera(Camera *camera) {
    _activeCamera = camera;
}

void Scene::addDirectionalLight(DirectionalLight *pLight) {
    _directionalLights.push_back(pLight);
}
