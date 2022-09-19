//
// Created by barth on 19/09/2022.
//

#include "Drawable.h"
#include "DefaultMaterial.h"

Drawable::Drawable(const char* name) : _name(name), _material(DefaultMaterial()), _vao(0), _vbo(0) {}

void Drawable::setVertexData(std::vector<float> *vertices) {
    _vertices = *vertices;

    glGenBuffers(1, &_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), _vertices.data(), GL_STATIC_DRAW);

    glGenVertexArrays(1, &_vao);
    glBindVertexArray(_vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, _vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
}

void Drawable::setMaterial(Material *material) {
    _material = *material;
}

void Drawable::render() {
    _material.bind();
    glBindVertexArray(_vao);
    // render points 0-3 from the currently bound VAO with current in-use shader
    glDrawArrays(GL_TRIANGLES, 0, 3);
}
