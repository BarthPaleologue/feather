//
// Created by barth on 03/10/2022.
//

#include "StandardMaterial.h"
#include "Settings.h"

StandardMaterial::StandardMaterial() : Material("./assets/shaders/standard") {
    std::string maxPointLights = std::to_string(Settings::MAX_POINT_LIGHTS);
    setDefine((std::string("MAX_POINT_LIGHTS ") + maxPointLights).c_str());
}

StandardMaterial::StandardMaterial(const char *diffuseTexturePath) : Material("./assets/shaders/standard") {
    setDiffuseTextureFromFile(diffuseTexturePath);
}

void StandardMaterial::setDiffuseTexture(Texture *texture) {
    if (_diffuseTexture == nullptr) setDefine("DIFFUSE_TEXTURE");
    _diffuseTexture = texture;
}

void StandardMaterial::setDiffuseTextureFromFile(const char *filePath) {
    Texture *diffuseTexture = new Texture(filePath);
    setDiffuseTexture(diffuseTexture);
}

void StandardMaterial::setAmbientTexture(Texture *texture) {
    if (_ambientTexture == nullptr) setDefine("AMBIENT_TEXTURE");
    _ambientTexture = texture;
}

void StandardMaterial::bind() {
    Material::bind();
    if (_diffuseTexture != nullptr) bindTexture("diffuseTexture", _diffuseTexture, 0);
    if (_ambientTexture != nullptr) bindTexture("ambientTexture", _ambientTexture, 1);
    setVec3("diffuseColor", _diffuseColor);
    setVec3("ambientColor", _ambientColor);
    if (_alphaColor != nullptr) setVec3("alphaColor", _alphaColor);
    setBool("lightingEnabled", _lightingEnabled);
}

void StandardMaterial::unbind() {
    Material::unbind();
    if (_diffuseTexture != nullptr) _diffuseTexture->unbind();
    if (_ambientTexture != nullptr) _ambientTexture->unbind();
}

void StandardMaterial::setAmbientColor(float r, float g, float b) {
    _ambientColor->x = r;
    _ambientColor->y = g;
    _ambientColor->z = b;
}

void StandardMaterial::setDiffuseColor(float r, float g, float b) {
    _diffuseColor->x = r;
    _diffuseColor->y = g;
    _diffuseColor->z = b;
}

void StandardMaterial::setAlphaColor(float r, float g, float b) {
    if (_alphaColor == nullptr) {
        setDefine("ALPHA_COLOR");
        _alphaColor = new glm::vec3(r, g, b);
    } else {
        _alphaColor->x = r;
        _alphaColor->y = g;
        _alphaColor->z = b;
    }
}

void StandardMaterial::setAmbientTextureFromFile(const char *filePath) {
    Texture *ambientTexture = new Texture(filePath);
    setAmbientTexture(ambientTexture);
}
