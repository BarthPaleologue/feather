//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_MESH_H
#define FEATHERGL_MESH_H

#include <vector>
#include <memory>
#include "Transformable.h"
#include "VertexData.h"
#include "DefaultMaterial.h"
#include "../utils/Uuid.h"
#include "Renderable.h"
#include "AABB.h"
#include "PickResult.h"

class Mesh : public Transformable, public Renderable {
public:
    explicit Mesh(const char *name);

    bool operator==(const Mesh &other) const {
        return _id == other._id;
    }

    const std::string &name() const {
        return _name;
    }

    Transform *transform() override {
        return &_transform;
    }

    AABB *aabb() {
        return &_aabb;
    }

    static std::shared_ptr<Mesh> FromVertexData(const char *name, VertexData &vertexData);

    void setMaterial(std::shared_ptr<Material> material);

    std::shared_ptr<Material> material() const {
        return _material;
    }

    void bakeTransformIntoVertexData();

    void bakeRotationIntoVertexData();

    void bakeScalingIntoVertexData();

    void setVertexData(VertexData &vertexData);

    void sendVertexDataToGPU();

    VertexData &vertexData() {
        return _vertexData;
    }

    PickResult pickWithRay(glm::vec3 rayOrigin, glm::vec3 rayDirection) {
        if (!_pickingEnabled) return PickResult{};
        glm::mat4 worldMatrix = transform()->computeWorldMatrix();
        return Utils::PickWithRay(vertexData().indices, vertexData().positions, rayOrigin, rayDirection, worldMatrix);
    }

    void render(glm::mat4 projectionViewMatrix, Shader* shaderOverride = nullptr);

    void setPickingEnabled(bool enabled) {
        _pickingEnabled = enabled;
    }

    bool isPickingEnabled() const {
        return _pickingEnabled;
    }

    void setEnabled(bool enabled) {
        _enabled = enabled;
    }

    bool isEnabled() {
        return _enabled;
    }


    static std::shared_ptr<Material> defaultMaterial;

private:
    std::string _name;
    std::string _id;
    VertexData _vertexData;

    Transform _transform;
    AABB _aabb;

    GLuint _vao{};
    GLuint _vbo{};
    GLuint _ibo{};
    GLuint _normalVbo{};
    GLuint _uvVbo{};
    GLuint _colVbo{};

    bool _pickingEnabled = true;
    bool _enabled = true;
protected:
    std::shared_ptr<Material> _material;
};

#endif //FEATHERGL_MESH_H
