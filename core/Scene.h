//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_SCENE_H
#define FEATHERGL_SCENE_H


#include "../meshes/AbstractMesh.h"
#include "cameras/Camera.h"
#include "lights/PointLight.h"

class Scene {
public:
    void addDrawable(AbstractMesh &mesh);

    void setActiveCamera(Camera *camera);

    void render(PointLight &light);

    Observable<> onBeforeRenderObservable{};
    Observable<> onAfterRenderObservable{};

private:
    std::vector<AbstractMesh> _meshes;
    Camera *_activeCamera = nullptr;
};


#endif //FEATHERGL_SCENE_H
