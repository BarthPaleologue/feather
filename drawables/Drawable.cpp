//
// Created by barth on 19/09/2022.
//

#include "Drawable.h"
#include "../materials/DefaultMaterial.h"

Drawable::Drawable(const char* name) : Transform(), _name(name), _material(DefaultMaterial()), _vao(0), _vbo(0) {}

void
Drawable::setVertexData(std::vector<GLfloat> *vertices, std::vector<GLint> *indices, std::vector<GLfloat> *colors) {
    _vertices = *vertices;
    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_DYNAMIC_READ);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    _indices = *indices;
    GLuint ibo = 0;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(float), _indices.data(), GL_DYNAMIC_READ);

    _colors = *colors;
    GLuint colVbo = 0;
    glGenBuffers(1, &colVbo);
    glBindBuffer(GL_ARRAY_BUFFER, colVbo);
    glBufferData(GL_ARRAY_BUFFER, _colors.size() * sizeof(float), _colors.data(), GL_DYNAMIC_READ);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(1);

}

void Drawable::setMaterial(Material *material) {
    _material = *material;
}

void Drawable::render(Camera &camera, PointLight &light) {
    _material.bind();

    const glm::mat4 viewMatrix = camera.getViewMatrix();
    const glm::mat4 projMatrix = camera.getProjectionMatrix();

    _material.setMat4("projection", &projMatrix);
    _material.setMat4("view", &viewMatrix);

    _material.setVec3("lightPosition", light.getPosition());
    _material.setVec3("worldPosition", &_position);

    glBindVertexArray(_vao);
    glDrawElements(GL_TRIANGLES, (int)_indices.size(), GL_UNSIGNED_INT, nullptr);
}