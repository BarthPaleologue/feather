//
// Created by barth on 03/10/2022.
//

#ifndef FEATHERGL_BLINNPHONGMATERIAL_H
#define FEATHERGL_BLINNPHONGMATERIAL_H

#include "Material.h"
#include "Texture.h"
#include "Camera.h"
#include "Scene.h"

class BlinnPhongMaterial : public Material {
public:
    explicit BlinnPhongMaterial(std::shared_ptr<Scene> scene);

    void setDiffuseTexture(std::shared_ptr<Texture> texture);

    void setDiffuseTextureFromFile(const char *filePath);

    void setAmbientTexture(std::shared_ptr<Texture> texture);

    void setAmbientTextureFromFile(const char *filePath);

    void setDiffuseColor(float r, float g, float b);

    void setAlphaColor(float r, float g, float b);

    void setAmbientColor(glm::vec3 *color) {
        *_ambientColor = *color;
    }

    void setAmbientColor(float r, float g, float b);

    void receiveShadows(std::shared_ptr<ShadowRenderer> shadowRenderer);

    void setLightingEnabled(bool enabled) { _lightingEnabled = enabled; }

    void setShininess(float shininess) { _shininess = shininess; }

    void bind() override;

    void unbind() override;

private:
    std::shared_ptr<Texture> _diffuseTexture = nullptr;
    std::shared_ptr<Texture> _ambientTexture = nullptr;
    glm::vec3 *_alphaColor = nullptr;
    glm::vec3 *_diffuseColor = new glm::vec3(0.0);
    glm::vec3 *_ambientColor = new glm::vec3(0.0);

    float _shininess = 8.0f;

    std::shared_ptr<ShadowRenderer> _shadowRenderer = nullptr;

    std::shared_ptr<Scene> _scene;

    bool _lightingEnabled = true;
};


#endif //FEATHERGL_BLINNPHONGMATERIAL_H
