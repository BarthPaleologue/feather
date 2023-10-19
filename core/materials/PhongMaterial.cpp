//
// Created by barth on 03/10/2022.
//

#include "PhongMaterial.h"
#include "Settings.h"

PhongMaterial::PhongMaterial(std::shared_ptr<Scene> scene) : Material("./assets/shaders/standard"), _scene(scene) {
    std::string maxPointLights = std::to_string(Settings::MAX_POINT_LIGHTS);
    shader()->setDefine((std::string("MAX_POINT_LIGHTS ") + maxPointLights).c_str());

    std::string maxDirectionalLights = std::to_string(Settings::MAX_DIRECTIONAL_LIGHTS);
    shader()->setDefine((std::string("MAX_DIRECTIONAL_LIGHTS ") + maxDirectionalLights).c_str());
}

void PhongMaterial::setDiffuseTexture(Texture *texture) {
    if (_diffuseTexture == nullptr) shader()->setDefine("DIFFUSE_TEXTURE");
    _diffuseTexture = texture;
}

void PhongMaterial::setDiffuseTextureFromFile(const char *filePath) {
    Texture *diffuseTexture = new Texture(filePath);
    setDiffuseTexture(diffuseTexture);
}

void PhongMaterial::setAmbientTexture(Texture *texture) {
    if (_ambientTexture == nullptr) shader()->setDefine("AMBIENT_TEXTURE");
    _ambientTexture = texture;
}

void PhongMaterial::setShadowMap(Texture *texture) {
    if (_shadowMap == nullptr) shader()->setDefine("SHADOW_MAP");
    _shadowMap = texture;
}

void PhongMaterial::bind() {
    Material::bind();

    shader()->setVec3("cameraPosition", _scene->activeCamera()->position());

    if (_diffuseTexture != nullptr) shader()->bindTexture("diffuseTexture", _diffuseTexture, 0);
    if (_ambientTexture != nullptr) shader()->bindTexture("ambientTexture", _ambientTexture, 1);
    if (_shadowMap != nullptr) shader()->bindTexture("shadowMap", _shadowMap, 2);
    shader()->setVec3("diffuseColor", _diffuseColor);
    shader()->setVec3("ambientColor", _ambientColor);
    if (_alphaColor != nullptr) shader()->setVec3("alphaColor", _alphaColor);
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

void PhongMaterial::unbind() {
    Material::unbind();
    if (_diffuseTexture != nullptr) _diffuseTexture->unbind();
    if (_ambientTexture != nullptr) _ambientTexture->unbind();
}

void PhongMaterial::setAmbientColor(float r, float g, float b) {
    _ambientColor->x = r;
    _ambientColor->y = g;
    _ambientColor->z = b;
}

void PhongMaterial::setDiffuseColor(float r, float g, float b) {
    _diffuseColor->x = r;
    _diffuseColor->y = g;
    _diffuseColor->z = b;
}

void PhongMaterial::setAlphaColor(float r, float g, float b) {
    if (_alphaColor == nullptr) {
        shader()->setDefine("ALPHA_COLOR");
        _alphaColor = new glm::vec3(r, g, b);
    } else {
        _alphaColor->x = r;
        _alphaColor->y = g;
        _alphaColor->z = b;
    }
}

void PhongMaterial::setAmbientTextureFromFile(const char *filePath) {
    Texture *ambientTexture = new Texture(filePath);
    setAmbientTexture(ambientTexture);
}
