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
    explicit PbrMaterial(std::shared_ptr<Scene> scene) : Material("./assets/shaders/pbr"), _scene(scene) {
        std::string maxPointLights = std::to_string(Settings::MAX_POINT_LIGHTS);
        shader()->setDefine((std::string("MAX_POINT_LIGHTS ") + maxPointLights).c_str());

        std::string maxDirectionalLights = std::to_string(Settings::MAX_DIRECTIONAL_LIGHTS);
        shader()->setDefine((std::string("MAX_DIRECTIONAL_LIGHTS ") + maxDirectionalLights).c_str());
    }

    void bind() override {
        Material::bind();

        if (_albedoTexture != nullptr) shader()->bindTexture("albedoTexture", _albedoTexture, 0);
        if (_roughnessTexture != nullptr) shader()->bindTexture("roughnessTexture", _roughnessTexture, 1);
        if (_normalTexture != nullptr) shader()->bindTexture("normalTexture", _normalTexture, 2);
        if (_shadowRenderer != nullptr) {
            shader()->bindTexture("shadowMap", _shadowRenderer->depthTexture(), 3);
            glm::mat4 lightSpaceMatrix = _shadowRenderer->projectionViewMatrix();
            shader()->setMat4("lightSpaceMatrix", &lightSpaceMatrix);
            shader()->setVec3("lightDirection", _shadowRenderer->directionalLight()->getDirection());
        }
        shader()->setVec3("cameraPosition", _scene->activeCamera()->position());
        shader()->setBool("lightingEnabled", _lightingEnabled);

        shader()->setVec3("albedoColor", _albedoColor);
        shader()->setVec3("ambientColor", _ambientColor);

        shader()->setFloat("metallic", metallic);
        shader()->setFloat("roughness", roughness);
        shader()->setFloat("ao", ao);

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

    void setMetallic(float metallic) { this->metallic = metallic; }

    void setRoughness(float roughness) { this->roughness = roughness; }

    void setRoughnessTexture(Texture *texture) {
        if (_roughnessTexture == nullptr) shader()->setDefine("ROUGHNESS_TEXTURE");
        _roughnessTexture = texture;
    }

    void receiveShadows(std::shared_ptr<ShadowRenderer> shadowRenderer) {
        if (_shadowRenderer == nullptr) shader()->setDefine("SHADOW_MAP");
        _shadowRenderer = shadowRenderer;
    }

    void setAlbedoColor(float r, float g, float b) {
        _albedoColor->x = r;
        _albedoColor->y = g;
        _albedoColor->z = b;
    }

    void setAlbedoTexture(Texture *texture) {
        if (_albedoTexture == nullptr) shader()->setDefine("ALBEDO_TEXTURE");
        _albedoTexture = texture;
    }

    void setNormalTexture(Texture *texture) {
        if (_normalTexture == nullptr) shader()->setDefine("NORMAL_TEXTURE");
        _normalTexture = texture;
    }

    void setAmbientColor(float r, float g, float b) {
        _ambientColor->x = r;
        _ambientColor->y = g;
        _ambientColor->z = b;
    }


private:
    std::shared_ptr<Scene> _scene;

    Texture *_albedoTexture = nullptr;
    Texture *_normalTexture = nullptr;
    Texture *_metallicTexture = nullptr;
    Texture *_roughnessTexture = nullptr;
    Texture *_aoTexture = nullptr;

    glm::vec3 *_albedoColor = new glm::vec3(1.0);
    glm::vec3 *_ambientColor = new glm::vec3(0.0);
    glm::vec3 *_alphaColor = nullptr;

    float metallic = 1.0f;
    float roughness = 0.5f;
    float ao = 1.0f;

    bool _lightingEnabled = true;

    std::shared_ptr<ShadowRenderer> _shadowRenderer = nullptr;
};

#endif //FEATHERGL_PBRMATERIAL_H
