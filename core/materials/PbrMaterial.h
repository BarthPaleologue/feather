//
// Created by barth on 19/10/23.
//

#ifndef FEATHERGL_PBRMATERIAL_H
#define FEATHERGL_PBRMATERIAL_H

#include "Material.h"
#include "Scene.h"
#include "Settings.h"

class PbrMaterial : public Material {
public:
    explicit PbrMaterial(std::shared_ptr<Scene> scene) : Material("./assets/shaders/standard"), _scene(scene) {
        std::string maxPointLights = std::to_string(Settings::MAX_POINT_LIGHTS);
        shader()->setDefine((std::string("MAX_POINT_LIGHTS ") + maxPointLights).c_str());

        std::string maxDirectionalLights = std::to_string(Settings::MAX_DIRECTIONAL_LIGHTS);
        shader()->setDefine((std::string("MAX_DIRECTIONAL_LIGHTS ") + maxDirectionalLights).c_str());
    }

    void bind() override {
        Material::bind();
        shader()->setVec3("cameraPosition", _scene->activeCamera()->position());

        shader()->setBool("lightingEnabled", _lightingEnabled);

        auto *pointLights = _scene->pointLights();
        shader()->setInt("pointLightCount", (int) pointLights->size());
        for (int i = 0; i < pointLights->size(); i++) {
            auto light = pointLights->at(i);
            shader()->setVec3(("pointLights[" + std::to_string(i) + "].position").c_str(),
                              light->transform()->position());
            shader()->setVec3(("pointLights[" + std::to_string(i) + "].color").c_str(), light->color());
            shader()->setFloat(("pointLights[" + std::to_string(i) + "].intensity").c_str(), light->intensity());
        }

        auto *directionalLights = _scene->directionalLights();
        shader()->setInt("directionalLightCount", (int) directionalLights->size());
        for (int i = 0; i < directionalLights->size(); i++) {
            auto light = directionalLights->at(i);
            shader()->setVec3(("directionalLights[" + std::to_string(i) + "].direction").c_str(),
                              light->getDirection());
            shader()->setVec3(("directionalLights[" + std::to_string(i) + "].color").c_str(),
                              light->color());
            shader()->setFloat(("directionalLights[" + std::to_string(i) + "].intensity").c_str(),
                               light->intensity());
        }
    }

    void unbind() override {
        Material::unbind();
        if (_shadowRenderer != nullptr) _shadowRenderer->depthTexture()->unbind();
    }

    void setLightingEnabled(bool enabled) { _lightingEnabled = enabled; }

    void receiveShadows(std::shared_ptr<ShadowRenderer> shadowRenderer) {
        if (_shadowRenderer == nullptr) shader()->setDefine("SHADOW_MAP");
        _shadowRenderer = shadowRenderer;
    }

private:
    std::shared_ptr<Scene> _scene;

    bool _lightingEnabled = true;

    std::shared_ptr<ShadowRenderer> _shadowRenderer = nullptr;
};

#endif //FEATHERGL_PBRMATERIAL_H
