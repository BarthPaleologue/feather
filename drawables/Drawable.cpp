//
// Created by barth on 19/09/2022.
//

#include "glm/ext.hpp"
#include "Drawable.h"
#include "../materials/DefaultMaterial.h"

Drawable::Drawable(const char* name) : _name(name), _material(DefaultMaterial()), _vao(0), _vbo(0), _position(glm::vec3(0.0f, 0.0f, 0.0f)) {}

void
Drawable::setVertexData(std::vector<GLfloat> *vertices, std::vector<GLint> *indices, std::vector<GLfloat> *colors) {
    _vertices = *vertices;
    _indices = *indices;
    _colors = *colors;

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_DYNAMIC_READ);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

    GLuint ibo = 0;
    glGenBuffers(1, &ibo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(float), _indices.data(), GL_DYNAMIC_READ);

    GLuint colVbo = 0;
    glGenBuffers(1, &colVbo);
    glBindBuffer(GL_ARRAY_BUFFER, colVbo);
    glBufferData(GL_ARRAY_BUFFER, _colors.size() * sizeof(float ), _colors.data(), GL_DYNAMIC_READ);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
    glEnableVertexAttribArray(1);

}

void Drawable::setMaterial(Material *material) {
    _material = *material;
}

void Drawable::render(Camera* camera) {
    _material.bind();

    const glm::mat4 viewMatrix = camera->computeViewMatrix();
    const glm::mat4 projMatrix = camera->computeProjectionMatrix();

    const glm::mat4 MVP = glm::translate(glm::mat4(1.0f), _position);
    _material.setMat4("MVP", &MVP);

    glBindVertexArray(_vao);
    glDrawArrays(GL_TRIANGLES, 0, _indices.size());
}

void Drawable::setPosition(glm::vec3 *newPosition) {
    _position.x = newPosition->x;
    _position.y = newPosition->y;
    _position.z = newPosition->z;
}

void Drawable::setPositionFromFloats(float newX, float newY, float newZ) {
    _position.x = newX;
    _position.y = newY;
    _position.z = newZ;
}
