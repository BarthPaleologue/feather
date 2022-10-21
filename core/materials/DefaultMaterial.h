//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_DEFAULTMATERIAL_H
#define FEATHERGL_DEFAULTMATERIAL_H


#include "Material.h"
#include "Texture.h"

class DefaultMaterial: public Material {
public:
    DefaultMaterial();
    void bind() override;
    void unbind() override;

private:
    Texture *_texture;
};


#endif //FEATHERGL_DEFAULTMATERIAL_H
