//
// Created by barth on 19/09/2022.
//

#include "Drawable.h"
#include "../materials/DefaultMaterial.h"
#include <iostream>

Drawable::Drawable(const char *name) : Transform(), _name(name), _vao(0), _vbo(0) {
    _material = new DefaultMaterial();
}

void
Drawable::setVertexData(std::vector<GLfloat> *vertices, std::vector<GLint> *indices, std::vector<GLfloat> *normals,
                        std::vector<GLfloat> *uvs, std::vector<GLfloat> *colors) {
    _vertices = *vertices;
    int vertexLayoutIndex = 0;
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_DYNAMIC_READ);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(vertexLayoutIndex, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(vertexLayoutIndex);

    _indices = *indices;
    GLuint ibo = 0;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(float), _indices.data(), GL_DYNAMIC_READ);

    _normals = *normals;
    int normalLayoutIndex = 2;
    GLuint normalVbo = 0;
    glGenBuffers(1, &normalVbo);
    glBindBuffer(GL_ARRAY_BUFFER, normalVbo);
    glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(float), _normals.data(), GL_DYNAMIC_READ);
    glVertexAttribPointer(normalLayoutIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(normalLayoutIndex);

    _uvs = *uvs;
    int uvLayoutIndex = 3;
    GLuint uvVbo = 0;
    glGenBuffers(1, &uvVbo);
    glBindBuffer(GL_ARRAY_BUFFER, uvVbo);
    glBufferData(GL_ARRAY_BUFFER, _uvs.size() * sizeof(float), _uvs.data(), GL_DYNAMIC_READ);
    glVertexAttribPointer(uvLayoutIndex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(uvLayoutIndex);

    _colors = *colors;
    int colorLayoutIndex = 1;
    GLuint colVbo = 0;
    glGenBuffers(1, &colVbo);
    glBindBuffer(GL_ARRAY_BUFFER, colVbo);
    glBufferData(GL_ARRAY_BUFFER, _colors.size() * sizeof(float), _colors.data(), GL_DYNAMIC_READ);
    glVertexAttribPointer(colorLayoutIndex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(colorLayoutIndex);
}

void Drawable::setMaterial(Material *material) {
    _material = material;
}

void Drawable::render(Camera &camera, PointLight &light) {
    _material->bind();

    const glm::mat4 viewMatrix = camera.getViewMatrix();
    const glm::mat4 projMatrix = camera.getProjectionMatrix();
    const glm::mat4 world = computeWorldMatrix();
    const glm::mat4 normalMatrix = glm::transpose(glm::inverse(world));

    _material->setMat4("projection", &projMatrix);
    _material->setMat4("view", &viewMatrix);
    _material->setMat4("world", &world);
    _material->setMat4("normalMatrix", &normalMatrix);

    _material->setVec3("lightPosition", light.getPosition());
    _material->setVec3("cameraPosition", camera.getPosition());

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, (int) _indices.size(), GL_UNSIGNED_INT, nullptr);
}

