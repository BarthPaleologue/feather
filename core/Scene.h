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
    explicit Scene(Engine *engine) {
        _engine = engine;

        _engine->onWindowResizeObservable.add([this](int width, int height) {
            this->_activeCamera->setAspectRatio((float) width / (float) height);
        });
    }

    void addMesh(Mesh *mesh);

    void addPointLight(PointLight *light) {
        _pointLights.push_back(light);
    }

    void addPostProcess(PostProcessing *postProcess) {
        _postProcesses.push_back(postProcess);
        _engine->onWindowResizeObservable.add([postProcess](int width, int height) {
            postProcess->resize(width, height);
        });
    }

    void setActiveCamera(Camera *camera);

    Camera *activeCamera() { return _activeCamera; }

    std::vector<PointLight *> *pointLights() {
        return &_pointLights;
    }

    std::vector<DirectionalLight *> *directionalLights() {
        return &_directionalLights;
    }

    void render();

    void compileShaders() {
        for (auto mesh: _meshes) {
            mesh->material()->compile();
        }
    }

    Observable<> onBeforeRenderObservable{};
    Observable<> onAfterRenderObservable{};

    void addDirectionalLight(DirectionalLight *pLight);

private:
    Engine *_engine;

    std::vector<PointLight *> _pointLights{};
    std::vector<DirectionalLight *> _directionalLights{};

    std::vector<Mesh *> _meshes{};

    std::vector<ShadowRenderer *> _shadowRenderers{};
    std::vector<PostProcessing *> _postProcesses{};

    Camera *_activeCamera = nullptr;
};


#endif //FEATHERGL_SCENE_H
