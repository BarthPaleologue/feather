//
// Created by barth on 18/10/23.
//

#ifndef FEATHERGL_SCREENQUAD_H
#define FEATHERGL_SCREENQUAD_H

#include <vector>

class ScreenQuad {
public:
    ScreenQuad() {
        std::vector<float> positions = {
                -1.0f, -1.0f, 0.0f,
                1.0f, -1.0f, 0.0f,
                -1.0f, 1.0f, 0.0f,
                1.0f, 1.0f, 0.0f
        };
        std::vector<float> uvs = {
                0.0f, 0.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 1.0f
        };

        int vertexLayoutIndex = 0;
        glGenBuffers(1, &_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(float), positions.data(),
                     GL_DYNAMIC_READ);

        glGenVertexArrays(1, &_vao);
        glBindVertexArray(_vao);
        glBindBuffer(GL_ARRAY_BUFFER, _vbo);
        glVertexAttribPointer(vertexLayoutIndex, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(vertexLayoutIndex);

        glGenBuffers(1, &_ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(),
                     GL_DYNAMIC_READ);

        int uvLayoutIndex = 1;
        glGenBuffers(1, &_uvVbo);
        glBindBuffer(GL_ARRAY_BUFFER, _uvVbo);
        glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(float), uvs.data(), GL_DYNAMIC_READ);
        glVertexAttribPointer(uvLayoutIndex, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
        glEnableVertexAttribArray(uvLayoutIndex);
    }

    void render() {
        glBindVertexArray(_vao);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
    }

private:
    GLuint _vao{};
    GLuint _vbo{};
    GLuint _ibo{};
    GLuint _uvVbo{};

    std::vector<int> indices = {
            0, 1, 2,
            1, 3, 2
    };
};

#endif //FEATHERGL_SCREENQUAD_H
