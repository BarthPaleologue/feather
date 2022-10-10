//
// Created by barth on 03/10/2022.
//

#include "StandardMaterial.h"

StandardMaterial::StandardMaterial() : Material("./assets/shaders/standard") {
    _diffuseColor = new glm::vec3(0.0);
    _emissiveColor = new glm::vec3(0.0);
}

void StandardMaterial::setDiffuseTexture(Texture *texture) {
    if(_diffuseTexture == nullptr) setDefine("DIFFUSE_TEXTURE");
    _diffuseTexture = texture;
    setTexture("diffuseTexture", _diffuseTexture, 0);
}

void StandardMaterial::setAmbientTexture(Texture *texture) {
    if(_emissiveTexture == nullptr) setDefine("AMBIENT_TEXTURE");
    _emissiveTexture = texture;
    setTexture("ambientTexture", _emissiveTexture, 1);
}

void StandardMaterial::bind() {
    Material::bind();
    if (_diffuseTexture != nullptr) _diffuseTexture->bind(0);
    setVec3("diffuseColor", _diffuseColor);
    setVec3("ambientColor", _emissiveColor);
    if(_alphaColor != nullptr) setVec3("alphaColor", _alphaColor);
}

void StandardMaterial::unbind() {
    Material::unbind();
    if (_diffuseTexture != nullptr) _diffuseTexture->unbind();
}

void StandardMaterial::setAmbientColor(float r, float g, float b) {
    _emissiveColor->x = r;
    _emissiveColor->y = g;
    _emissiveColor->z = b;
}

void StandardMaterial::setDiffuseColor(float r, float g, float b) {
    _diffuseColor->x = r;
    _diffuseColor->y = g;
    _diffuseColor->z = b;
}

void StandardMaterial::setAlphaColor(float r, float g, float b) {
    if(_alphaColor == nullptr) {
        setDefine("ALPHA_COLOR");
        _alphaColor = new glm::vec3(r, g, b);
    } else {
        _alphaColor->x = r;
        _alphaColor->y = g;
        _alphaColor->z = b;
    }
}
