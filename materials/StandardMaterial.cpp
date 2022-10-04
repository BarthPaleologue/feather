//
// Created by barth on 03/10/2022.
//

#include "StandardMaterial.h"

StandardMaterial::StandardMaterial() : Material("./assets/shaders/standard") {}

void StandardMaterial::setDiffuseTexture(Texture *texture) {
    if(_diffuseTexture == nullptr) setDefine("DIFFUSE_TEXTURE");
    _diffuseTexture = texture;
    setTexture("diffuseTexture", _diffuseTexture, 0);
}

void StandardMaterial::bind() {
    Material::bind();
    if (_diffuseTexture != nullptr) _diffuseTexture->bind(0);
    setVec3("emissiveColor", _emissiveColor);
}

void StandardMaterial::unbind() {
    Material::unbind();
    if (_diffuseTexture != nullptr) _diffuseTexture->unbind();
}

void StandardMaterial::setEmissiveColor(float r, float g, float b) {
    _emissiveColor->x = r;
    _emissiveColor->y = g;
    _emissiveColor->z = b;
}
