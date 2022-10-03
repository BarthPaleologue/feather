//
// Created by barth on 19/09/2022.
//

#include "DefaultMaterial.h"

DefaultMaterial::DefaultMaterial() : Material("./assets/shaders/default") {
    _texture = new Texture("assets/textures/debug.png");
    setTexture("texture", _texture, 0);
}

void DefaultMaterial::bind() {
    Material::bind();
    _texture->bind(0);
}

void DefaultMaterial::unbind() {
    Material::unbind();
    _texture->unbind();
}
