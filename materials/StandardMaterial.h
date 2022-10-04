//
// Created by barth on 03/10/2022.
//

#ifndef FEATHERGL_STANDARDMATERIAL_H
#define FEATHERGL_STANDARDMATERIAL_H

#include "Material.h"
#include "Texture.h"

class StandardMaterial : public Material {
public:
    StandardMaterial();

    void setDiffuseTexture(Texture *texture);

    void setEmissiveColor(float r, float g, float b);

    void bind() override;

    void unbind() override;

private:
    Texture *_diffuseTexture = nullptr;
    glm::vec3 *_emissiveColor = new glm::vec3(0.0);
};


#endif //FEATHERGL_STANDARDMATERIAL_H
