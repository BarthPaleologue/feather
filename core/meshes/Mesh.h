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

class Mesh : public Transformable {
public:
    explicit Mesh(const char *name) : Transformable(), _name(name), _vao(0), _vbo(0) {
        _id = UUID::generate_uuid_v4();
        _material = std::make_shared<DefaultMaterial>();
    }

    bool operator==(const Mesh &other) const {
        return _id == other._id;
    }

    static std::shared_ptr<Mesh> FromVertexData(const char *name, VertexData &vertexData);

    void setMaterial(std::shared_ptr<Material> material);

    std::shared_ptr<Material> material() const {
        return _material;
    }

    void bakeTransformIntoVertexData();

    void setVertexData(VertexData &vertexData);

    void sendVertexDataToGPU();

    VertexData &vertexData() {
        return _vertexData;
    }

    void render(glm::mat4 projectionViewMatrix, Shader* shaderOverride = nullptr);

private:
    std::string _name;
    std::string _id;
    VertexData _vertexData;

    GLuint _vao;
    GLuint _vbo;
    GLuint _ibo;
    GLuint _normalVbo;
    GLuint _uvVbo;
    GLuint _colVbo;
protected:
    std::shared_ptr<Material> _material;
};

#endif //FEATHERGL_MESH_H
