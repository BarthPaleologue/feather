//
// Created by barth on 03/10/2022.
//

#ifndef FEATHERGL_STANDARDMATERIAL_H
#define FEATHERGL_STANDARDMATERIAL_H

#include "Material.h"
#include "Texture.h"

class StandardMaterial: public Material {
public:
    StandardMaterial();
    void setDiffuseTexture(Texture *texture);
    void bind() override;
    void unbind() override;

private:
    Texture* _diffuseTexture = nullptr;
};


#endif //FEATHERGL_STANDARDMATERIAL_H
