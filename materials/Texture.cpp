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


