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

class Mesh : public Transformable, public Renderable {
public:
    explicit Mesh(const char *name) : Transformable(), Renderable(), _name(name) {
        _id = UUID::generate_uuid_v4();
        _material = std::make_shared<DefaultMaterial>();
    }

    bool operator==(const Mesh &other) const {
        return _id == other._id;
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

    void render(glm::mat4 projectionViewMatrix, Shader* shaderOverride = nullptr);

    void setEnabled(bool enabled) {
        _enabled = enabled;
    }

    bool isEnabled() {
        return _enabled;
    }

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

    bool _enabled = true;
protected:
    std::shared_ptr<Material> _material;
};

#endif //FEATHERGL_MESH_H
