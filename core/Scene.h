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
    }

    void addMesh(Mesh *mesh);

    void setActiveCamera(Camera *camera);

    void render(PointLight &light);

    Observable<> onBeforeRenderObservable{};
    Observable<> onAfterRenderObservable{};

private:
    Engine *_engine;

    std::vector<Mesh *> _meshes;
    Camera *_activeCamera = nullptr;
};


#endif //FEATHERGL_SCENE_H
