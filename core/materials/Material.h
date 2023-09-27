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

class Material {
public:
    explicit Material(const char *shaderFolder);

    void compile();

    virtual void bind();

    virtual void unbind();

    void setMat4(const char *uniformName, const glm::mat4 *matrix) const;

    void setVec3(const char *uniformName, const glm::vec3 *vector) const;

    void setInt(const char *uniformName, int integer) const;

    void setFloat(const char *uniformName, float value) const;

    void bindTexture(const char *uniformName, Texture *texture, int id) const;

    void setDefine(const char *defineName);

    bool setWireframe(bool enabled) {
        _isWireframe = enabled;
    }

    bool wireframe() const {
        return _isWireframe;
    }

    bool isBackFaceCullingEnabled() const {
        return _isBackFaceCullingEnabled;
    }

    bool setBackFaceCullingEnabled(bool enabled) {
        _isBackFaceCullingEnabled = enabled;
    }

    Observable<> onBindObservable{};

private:
    GLuint _program;
    std::string _vertexShaderCode;
    std::string _fragmentShaderCode;

    bool _isWireframe = false;
    bool _isBackFaceCullingEnabled = true;
};


#endif //FEATHERGL_MATERIAL_H
