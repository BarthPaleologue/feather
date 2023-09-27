//
// Created by barth on 19/09/2022.
//

#include "Mesh.h"
#include "../materials/DefaultMaterial.h"
#include <iostream>

Mesh::Mesh(const char *name) : Transformable(), _name(name), _vao(0), _vbo(0) {
    _material = new DefaultMaterial();
}

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

    GLuint ibo = 0;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _vertexData.indices.size() * sizeof(float), _vertexData.indices.data(),
                 GL_DYNAMIC_READ);

    int normalLayoutIndex = 2;
    GLuint normalVbo = 0;
    glGenBuffers(1, &normalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glBufferData(GL_ARRAY_BUFFER, _vertexData.normals.size() * sizeof(float), _vertexData.normals.data(),
                 GL_DYNAMIC_READ);
    glVertexAttribPointer(normalLayoutIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(normalLayoutIndex);

    int uvLayoutIndex = 3;
    GLuint uvVbo = 0;
    glGenBuffers(1, &uvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glBufferData(GL_ARRAY_BUFFER, _vertexData.uvs.size() * sizeof(float), _vertexData.uvs.data(), GL_DYNAMIC_READ);
    glVertexAttribPointer(uvLayoutIndex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(uvLayoutIndex);

    int colorLayoutIndex = 1;
    GLuint colVbo = 0;
    glGenBuffers(1, &colVbo);
    glBindBuffer(GL_ARRAY_BUFFER, colVbo);
    glBufferData(GL_ARRAY_BUFFER, _vertexData.colors.size() * sizeof(float), _vertexData.colors.data(),
                 GL_DYNAMIC_READ);
    glVertexAttribPointer(colorLayoutIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(colorLayoutIndex);
}

void Mesh::setMaterial(Material *material) {
    _material = material;
}

void Mesh::render(Camera *camera, std::vector<PointLight *> &lights) {
    _material->bind();

    const glm::mat4 viewMatrix = camera->getViewMatrix();
    const glm::mat4 projMatrix = camera->getProjectionMatrix();
    const glm::mat4 world = transform()->computeWorldMatrix();
    const glm::mat4 normalMatrix = glm::transpose(glm::inverse(world));

    _material->setMat4("projection", &projMatrix);
    _material->setMat4("view", &viewMatrix);
    _material->setMat4("world", &world);
    _material->setMat4("normalMatrix", &normalMatrix);

    _material->setInt("pointLightCount", (int) lights.size());
    for(int i = 0; i < lights.size(); i++) {
        _material->setVec3(("pointLights[" + std::to_string(i) + "].position").c_str(),
                           lights[i]->transform()->position());
        _material->setVec3(("pointLights[" + std::to_string(i) + "].color").c_str(), lights[i]->color());
        _material->setFloat(("pointLights[" + std::to_string(i) + "].intensity").c_str(), lights[i]->intensity());
    }

    _material->setVec3("cameraPosition", camera->position());

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, (int) _vertexData.indices.size(), GL_UNSIGNED_INT, nullptr);

    _material->unbind();
}

Mesh *Mesh::FromVertexData(const char *name, VertexData &vertexData) {
    Mesh *mesh = new Mesh(name);
    mesh->setVertexData(vertexData);
    return mesh;
}

