//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_MATERIAL_H
#define FEATHERGL_MATERIAL_H


#include <string>
#include "glad/glad.h"
#include "glm/ext/matrix_float4x4.hpp"
#include "Texture.h"
#include "Observable.h"
#include "Shader.h"

class Material : public Shader {
public:
    explicit Material(const char *shaderFolder);

    void bind() override;

    void setWireframe(bool enabled) {
        _isWireframe = enabled;
    }

    bool wireframe() const {
        return _isWireframe;
    }

    bool isBackFaceCullingEnabled() const {
        return _isBackFaceCullingEnabled;
    }

    void setBackFaceCullingEnabled(bool enabled) {
        _isBackFaceCullingEnabled = enabled;
    }

private:
    bool _isWireframe = false;
    bool _isBackFaceCullingEnabled = true;
};


#endif //FEATHERGL_MATERIAL_H
