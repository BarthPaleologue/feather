//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_SCENE_H
#define FEATHERGL_SCENE_H


#include "../meshes/Mesh.h"
#include "cameras/Camera.h"
#include "lights/PointLight.h"
#include "postprocessing.h"
#include "shadows/ShadowRenderer.h"
#include "DirectionalLight.h"

class Scene {
public:
    explicit Scene(std::shared_ptr<Engine> engine) {
        _engine = engine;

        _engine->onWindowResizeObservable.add([this](int width, int height) {
            this->_activeCamera->setAspectRatio((float) width / (float) height);
        });
    }

    void addMesh(std::shared_ptr<Mesh> mesh);

    void addPointLight(std::shared_ptr<PointLight> light) {
        _pointLights.push_back(light);
    }

    void addPostProcess(std::shared_ptr<PostProcessing> postProcess) {
        _postProcesses.push_back(postProcess);
        _engine->onWindowResizeObservable.add([postProcess](int width, int height) {
            postProcess->resize(width, height);
        });
    }

    void addShadowRenderer(std::shared_ptr<ShadowRenderer> shadowRenderer) {
        _shadowRenderers.push_back(shadowRenderer);
    }

    void setActiveCamera(std::shared_ptr<Camera> camera);

    std::shared_ptr<Camera> activeCamera() { return _activeCamera; }

    std::vector<std::shared_ptr<PointLight>> *pointLights() {
        return &_pointLights;
    }

    std::vector<std::shared_ptr<DirectionalLight>> *directionalLights() {
        return &_directionalLights;
    }

    void render();

    void compileShaders() {
        for (auto mesh: _meshes) {
            mesh->material()->shader()->compile();
        }
    }

    Observable<> onBeforeRenderObservable{};
    Observable<> onAfterRenderObservable{};

    void addDirectionalLight(std::shared_ptr<DirectionalLight> pLight);

private:
    std::shared_ptr<Engine> _engine;

    std::vector<std::shared_ptr<PointLight>> _pointLights{};
    std::vector<std::shared_ptr<DirectionalLight>> _directionalLights{};

    std::vector<std::shared_ptr<Mesh>> _meshes{};

    std::vector<std::shared_ptr<ShadowRenderer>> _shadowRenderers{};
    std::vector<std::shared_ptr<PostProcessing>> _postProcesses{};

    std::shared_ptr<Camera> _activeCamera = nullptr;
};


#endif //FEATHERGL_SCENE_H
