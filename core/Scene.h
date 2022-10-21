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
    void render(Camera &camera, PointLight &light);

private:
    std::vector<AbstractMesh> _meshes;
};


#endif //FEATHERGL_SCENE_H
