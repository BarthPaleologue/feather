//
// Created by barth on 03/10/2022.
//

#include "StandardMaterial.h"
#include <iostream>

StandardMaterial::StandardMaterial() : Material("./assets/shaders/standard") {}

void StandardMaterial::setDiffuseTexture(Texture *texture) {
    _diffuseTexture = texture;
    setTexture("diffuseTexture", _diffuseTexture, 0);
}

void StandardMaterial::bind() {
    Material::bind();
    _diffuseTexture->bind(0);
}

void StandardMaterial::unbind() {
    Material::unbind();
    _diffuseTexture->unbind();
}
