//
// Created by barth on 19/09/2022.
//

#include "DefaultMaterial.h"

DefaultMaterial::DefaultMaterial() : Material("./assets/shaders/default") {
    _texture = new Texture("assets/textures/debug.png");
    bindTexture("texture", _texture, 0);
}

void DefaultMaterial::bind() {
    Material::bind();
    bindTexture("texture", _texture, 0);
}

void DefaultMaterial::unbind() {
    Material::unbind();
    _texture->unbind();
}
