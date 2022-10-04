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

    void setEmissiveTexture(Texture *texture);

    void setDiffuseColor(float r, float g, float b);

    void setAlphaColor(float r, float g, float b);

    void setEmissiveColor(float r, float g, float b);

    void bind() override;

    void unbind() override;

private:
    Texture *_diffuseTexture = nullptr;
    Texture *_emissiveTexture = nullptr;
    glm::vec3 *_alphaColor = nullptr;
    glm::vec3 *_diffuseColor;
    glm::vec3 *_emissiveColor;
};


#endif //FEATHERGL_STANDARDMATERIAL_H
