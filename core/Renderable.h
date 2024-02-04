//
// Created by barth on 04/02/24.
//

#ifndef FEATHERGL_RENDERABLE_H
#define FEATHERGL_RENDERABLE_H

#include <glm/glm.hpp>
#include "Shader.h"

class Renderable {
public:
    virtual void render(glm::mat4 projectionViewMatrix, Shader* shaderOverride = nullptr) = 0;
    virtual void setEnabled(bool enabled) = 0;
    virtual bool isEnabled() = 0;
    virtual ~Renderable() = default;

};

#endif //FEATHERGL_RENDERABLE_H
