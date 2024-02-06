//
// Created by barth on 19/09/2022.
//

#include "Scene.h"
#include "DepthMaterial.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_opengl3.h"
#include "../imgui/imgui_impl_glfw.h"

void Scene::addMesh(std::shared_ptr<Mesh> mesh) {
    _meshes.push_back(mesh);
}

void Scene::render() {
    onBeforeRenderObservable.notifyObservers();

    // Shadow pass
    for (const auto &shadowRenderer: _shadowRenderers) {
        shadowRenderer->render();
    }

    if (!_postProcesses.empty()) {
        glBindFramebuffer(GL_FRAMEBUFFER, _postProcesses[0]->getFBO());
    } else {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (const auto& _drawable: _meshes) {
        _drawable->render(_activeCamera->projectionViewMatrix());
    }

    if (!_postProcesses.empty()) {
        for (int i = 0; i < _postProcesses.size() - 1; i++) {
            _postProcesses[i]->RenderTo(_postProcesses[i + 1]->getFBO());
        }
        _postProcesses[_postProcesses.size() - 1]->RenderToScreen();
    }

    // Render UI on top of everything
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    onRenderGuiObservable.notifyObservers();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    onAfterRenderObservable.notifyObservers();
}

void Scene::setActiveCamera(std::shared_ptr<Camera> camera) {
    _activeCamera = camera;
}

void Scene::addDirectionalLight(std::shared_ptr<DirectionalLight> pLight) {
    _directionalLights.push_back(pLight);
}
