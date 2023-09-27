//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_SCENE_H
#define FEATHERGL_SCENE_H


#include "../meshes/Mesh.h"
#include "cameras/Camera.h"
#include "lights/PointLight.h"

class Scene {
public:
    explicit Scene(Engine *engine) {
        _engine = engine;

        _engine->onWindowResizeObservable.add([this](int width, int height) {
            this->_activeCamera->setAspectRatio(static_cast<float>(width) / static_cast<float>(height));
        });

        onBeforeRenderObservable.addOnce([this]() {
            compileShaders();
        });
    }

    void addMesh(Mesh *mesh);

    void addPointLight(PointLight *light) {
        _pointLights.push_back(light);
    }

    void setActiveCamera(Camera *camera);

    void render();

    void compileShaders() {
        for (auto mesh: _meshes) {
            mesh->material()->compile();
        }
    }

    Observable<> onBeforeRenderObservable{};
    Observable<> onAfterRenderObservable{};

private:
    Engine *_engine;

    std::vector<PointLight *> _pointLights;
    std::vector<Mesh *> _meshes;
    Camera *_activeCamera = nullptr;
};


#endif //FEATHERGL_SCENE_H
