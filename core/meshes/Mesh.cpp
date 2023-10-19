//
// Created by barth on 19/09/2022.
//

#include "Mesh.h"
#include <iostream>

void Mesh::setVertexData(VertexData &vertexData) {
    _vertexData = vertexData;

    int vertexLayoutIndex = 0;
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertexData.positions.size() * sizeof(float), _vertexData.positions.data(),
                 GL_DYNAMIC_READ);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(vertexLayoutIndex, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(vertexLayoutIndex);

    glGenBuffers(1, &_ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _vertexData.indices.size() * sizeof(float), _vertexData.indices.data(),
                 GL_DYNAMIC_READ);

    int normalLayoutIndex = 2;
    glGenBuffers(1, &_normalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _normalVbo);
    glBufferData(GL_ARRAY_BUFFER, _vertexData.normals.size() * sizeof(float), _vertexData.normals.data(),
                 GL_DYNAMIC_READ);
    glVertexAttribPointer(normalLayoutIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(normalLayoutIndex);

    int uvLayoutIndex = 3;
    glGenBuffers(1, &_uvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _uvVbo);
    glBufferData(GL_ARRAY_BUFFER, _vertexData.uvs.size() * sizeof(float), _vertexData.uvs.data(), GL_DYNAMIC_READ);
    glVertexAttribPointer(uvLayoutIndex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(uvLayoutIndex);

    int colorLayoutIndex = 1;
    glGenBuffers(1, &_colVbo);
    glBindBuffer(GL_ARRAY_BUFFER, _colVbo);
    glBufferData(GL_ARRAY_BUFFER, _vertexData.colors.size() * sizeof(float), _vertexData.colors.data(),
                 GL_DYNAMIC_READ);
    glVertexAttribPointer(colorLayoutIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(colorLayoutIndex);
}

void Mesh::updateVertexData() {
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertexData.positions.size() * sizeof(float), _vertexData.positions.data(),
                 GL_DYNAMIC_READ);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _vertexData.indices.size() * sizeof(float), _vertexData.indices.data(),
                 GL_DYNAMIC_READ);

    glBindBuffer(GL_ARRAY_BUFFER, _normalVbo);
    glBufferData(GL_ARRAY_BUFFER, _vertexData.normals.size() * sizeof(float), _vertexData.normals.data(),
                 GL_DYNAMIC_READ);

    glBindBuffer(GL_ARRAY_BUFFER, _uvVbo);
    glBufferData(GL_ARRAY_BUFFER, _vertexData.uvs.size() * sizeof(float), _vertexData.uvs.data(), GL_DYNAMIC_READ);

    glBindBuffer(GL_ARRAY_BUFFER, _colVbo);
    glBufferData(GL_ARRAY_BUFFER, _vertexData.colors.size() * sizeof(float), _vertexData.colors.data(),
                 GL_DYNAMIC_READ);
}

void Mesh::setMaterial(std::shared_ptr<Material> material) {
    _material = material;
}

void Mesh::render(glm::mat4 projectionViewMatrix, std::shared_ptr<Shader> shaderOverride) {
    auto shader = shaderOverride == nullptr ? _material->shader() : shaderOverride;

    if (shaderOverride == nullptr) _material->bind();
    else shader->bind();

    const glm::mat4 world = transform()->computeWorldMatrix();
    const glm::mat4 normalMatrix = transform()->computeNormalMatrix();

    shader->setMat4("projectionView", &projectionViewMatrix);
    shader->setMat4("world", &world);
    shader->setMat4("normalMatrix", &normalMatrix);

    glBindVertexArray(_vao);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
    glDrawElements(GL_TRIANGLES, _vertexData.indices.size(), GL_UNSIGNED_INT, nullptr);

    if (shaderOverride == nullptr) _material->unbind();
    else shader->unbind();
}

std::shared_ptr<Mesh> Mesh::FromVertexData(const char *name, VertexData &vertexData) {
    auto mesh = std::make_shared<Mesh>(name);
    mesh->setVertexData(vertexData);
    return mesh;
}

void Mesh::bakeTransformIntoVertexData() {
    auto worldMatrix = transform()->computeWorldMatrix();
    auto normalMatrix = transform()->computeNormalMatrix();
    for (int i = 0; i < _vertexData.positions.size(); i += 3) {
        glm::vec4 position = glm::vec4(_vertexData.positions[i], _vertexData.positions[i + 1],
                                       _vertexData.positions[i + 2], 1.0f);
        glm::vec4 transformedPosition = worldMatrix * position;
        _vertexData.positions[i] = transformedPosition.x;
        _vertexData.positions[i + 1] = transformedPosition.y;
        _vertexData.positions[i + 2] = transformedPosition.z;

        glm::vec4 normal = glm::vec4(_vertexData.normals[i], _vertexData.normals[i + 1],
                                     _vertexData.normals[i + 2], 1.0f);
        glm::vec4 transformedNormal = normalMatrix * normal;
        _vertexData.normals[i] = transformedNormal.x;
        _vertexData.normals[i + 1] = transformedNormal.y;
        _vertexData.normals[i + 2] = transformedNormal.z;
    }

    updateVertexData();

    transform()->reset();
}

