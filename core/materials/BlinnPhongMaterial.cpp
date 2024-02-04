//
// Created by barth on 03/10/2022.
//

#include "BlinnPhongMaterial.h"
#include "Settings.h"

BlinnPhongMaterial::BlinnPhongMaterial(std::shared_ptr<Scene> scene) : Material("./assets/shaders/blinnPhong"),
                                                                       _scene(scene) {
    std::string maxPointLights = std::to_string(Settings::MAX_POINT_LIGHTS);
    shader()->setDefine((std::string("MAX_POINT_LIGHTS ") + maxPointLights).c_str());

    std::string maxDirectionalLights = std::to_string(Settings::MAX_DIRECTIONAL_LIGHTS);
    shader()->setDefine((std::string("MAX_DIRECTIONAL_LIGHTS ") + maxDirectionalLights).c_str());
}

void BlinnPhongMaterial::setDiffuseTexture(std::shared_ptr<Texture> texture) {
    if (_diffuseTexture == nullptr) shader()->setDefine("DIFFUSE_TEXTURE");
    _diffuseTexture = texture;
}

void BlinnPhongMaterial::setDiffuseTextureFromFile(const char *filePath) {
    auto diffuseTexture = std::make_shared<Texture>(filePath);
    setDiffuseTexture(diffuseTexture);
}

void BlinnPhongMaterial::setAmbientTexture(std::shared_ptr<Texture> texture) {
    if (_ambientTexture == nullptr) shader()->setDefine("AMBIENT_TEXTURE");
    _ambientTexture = texture;
}

void BlinnPhongMaterial::receiveShadows(std::shared_ptr<ShadowRenderer> shadowRenderer) {
    if (_shadowRenderer == nullptr) shader()->setDefine("SHADOW_MAP");
    _shadowRenderer = shadowRenderer;
}

void BlinnPhongMaterial::bind() {
    Material::bind();

    shader()->setVec3("cameraPosition", _scene->activeCamera()->position());

    shader()->setFloat("shininess", _shininess);

    if (_diffuseTexture != nullptr) shader()->bindTexture("diffuseTexture", _diffuseTexture.get(), 0);
    if (_ambientTexture != nullptr) shader()->bindTexture("ambientTexture", _ambientTexture.get(), 1);
    if (_shadowRenderer != nullptr) {
        shader()->bindTexture("shadowMap", _shadowRenderer->depthTexture(), 2);
        glm::mat4 lightSpaceMatrix = _shadowRenderer->projectionViewMatrix();
        shader()->setMat4("lightSpaceMatrix", &lightSpaceMatrix);
        shader()->setVec3("lightDirection", _shadowRenderer->directionalLight()->getDirection());
    }
    shader()->setVec3("diffuseColor", _diffuseColor);
    shader()->setVec3("ambientColor", _ambientColor);
    if (_hasAlphaColor) shader()->setVec3("alphaColor", _alphaColor);
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

void BlinnPhongMaterial::unbind() {
    Material::unbind();
    if (_diffuseTexture != nullptr) _diffuseTexture->unbind();
    if (_ambientTexture != nullptr) _ambientTexture->unbind();
    if (_shadowRenderer != nullptr) _shadowRenderer->depthTexture()->unbind();
}

void BlinnPhongMaterial::setAmbientColor(float r, float g, float b) {
    _ambientColor.x = r;
    _ambientColor.y = g;
    _ambientColor.z = b;
}

void BlinnPhongMaterial::setDiffuseColor(float r, float g, float b) {
    _diffuseColor.x = r;
    _diffuseColor.y = g;
    _diffuseColor.z = b;
}

void BlinnPhongMaterial::setAlphaColor(float r, float g, float b) {
    if (!_hasAlphaColor) {
        shader()->setDefine("ALPHA_COLOR");
        _hasAlphaColor = true;
    }
    _alphaColor.x = r;
    _alphaColor.y = g;
    _alphaColor.z = b;
}

void BlinnPhongMaterial::setAmbientTextureFromFile(const char *filePath) {
    auto ambientTexture = std::make_shared<Texture>(filePath);
    setAmbientTexture(ambientTexture);
}
