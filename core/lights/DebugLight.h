//
// Created by barth on 27/09/23.
//

#ifndef FEATHERGL_DEBUGLIGHT_H
#define FEATHERGL_DEBUGLIGHT_H

#include "Mesh.h"
#include "Scene.h"
#include "MeshBuilder.h"
#include "PhongMaterial.h"

class DebugLight {
public:
    static Mesh *CreateDebugPointLightMesh(PointLight *light, Scene &scene) {
        auto mesh = MeshBuilder::makeCube("debug_light_sphere", scene);
        mesh->transform()->setScale(0.3f);
        mesh->transform()->setParent(light->transform());

        auto material = new PhongMaterial();
        material->setLightingEnabled(false);
        material->setAmbientColor(light->color());
        light->onColorChanged.add([material](float r, float g, float b) {
            material->setAmbientColor(r, g, b);
        });

        mesh->setMaterial(material);
    }
};

#endif //FEATHERGL_DEBUGLIGHT_H
