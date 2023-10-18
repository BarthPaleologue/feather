//
// Created by barth on 03/10/2022.
//

#include "PhongMaterial.h"
#include "Settings.h"

PhongMaterial::PhongMaterial(Scene *scene) : Material("./assets/shaders/standard"), _scene(scene) {
    std::string maxPointLights = std::to_string(Settings::MAX_POINT_LIGHTS);
    setDefine((std::string("MAX_POINT_LIGHTS ") + maxPointLights).c_str());

    std::string maxDirectionalLights = std::to_string(Settings::MAX_DIRECTIONAL_LIGHTS);
    setDefine((std::string("MAX_DIRECTIONAL_LIGHTS ") + maxDirectionalLights).c_str());
}

void PhongMaterial::setDiffuseTexture(Texture *texture) {
    if (_diffuseTexture == nullptr) setDefine("DIFFUSE_TEXTURE");
    _diffuseTexture = texture;
}

void PhongMaterial::setDiffuseTextureFromFile(const char *filePath) {
    Texture *diffuseTexture = new Texture(filePath);
    setDiffuseTexture(diffuseTexture);
}

void PhongMaterial::setAmbientTexture(Texture *texture) {
    if (_ambientTexture == nullptr) setDefine("AMBIENT_TEXTURE");
    _ambientTexture = texture;
}

void PhongMaterial::bind() {
    Material::bind();

    setVec3("cameraPosition", _scene->activeCamera()->position());

    if (_diffuseTexture != nullptr) bindTexture("diffuseTexture", _diffuseTexture, 0);
    if (_ambientTexture != nullptr) bindTexture("ambientTexture", _ambientTexture, 1);
    setVec3("diffuseColor", _diffuseColor);
    setVec3("ambientColor", _ambientColor);
    if (_alphaColor != nullptr) setVec3("alphaColor", _alphaColor);
    setBool("lightingEnabled", _lightingEnabled);

    std::vector<PointLight *> *pointLights = _scene->pointLights();
    setInt("pointLightCount", (int) pointLights->size());
    for (int i = 0; i < pointLights->size(); i++) {
        auto light = pointLights->at(i);
        setVec3(("pointLights[" + std::to_string(i) + "].position").c_str(),
                light->transform()->position());
        setVec3(("pointLights[" + std::to_string(i) + "].color").c_str(), light->color());
        setFloat(("pointLights[" + std::to_string(i) + "].intensity").c_str(), light->intensity());
    }

    std::vector<DirectionalLight *> *directionalLights = _scene->directionalLights();
    setInt("directionalLightCount", (int) directionalLights->size());
    for (int i = 0; i < directionalLights->size(); i++) {
        auto light = directionalLights->at(i);
        setVec3(("directionalLights[" + std::to_string(i) + "].direction").c_str(),
                light->getDirection());
        setVec3(("directionalLights[" + std::to_string(i) + "].color").c_str(),
                light->color());
        setFloat(("directionalLights[" + std::to_string(i) + "].intensity").c_str(),
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
        setDefine("ALPHA_COLOR");
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
