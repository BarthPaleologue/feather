//
// Created by barth on 19/09/2022.
//

#include <iostream>
#include "glm/ext.hpp"
#include "Material.h"
#include "glad/glad.h"

Material::Material(const char *shaderFolder) : Shader(shaderFolder) {}

void Material::bind() {
    Shader::bind();

    if (_isBackFaceCullingEnabled) {
        glEnable(GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
    } else {
        glDisable(GL_CULL_FACE);
    }

    glPolygonMode(GL_FRONT_AND_BACK, _isWireframe ? GL_LINE : GL_FILL);
}