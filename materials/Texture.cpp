//
// Created by barth on 30/09/2022.
//

#include "Texture.h"

Texture::Texture(const char *filepath) {
    _id = loadTextureFromFileToGPU(filepath);
}

int Texture::getId() {
    return _id;
}

void Texture::bind(int texShaderId) {
    glActiveTexture(GL_TEXTURE0 + texShaderId);
    glBindTexture(GL_TEXTURE_2D, _id);
}

void Texture::unbind() {
    glBindTexture(GL_TEXTURE_2D, 0);
}


