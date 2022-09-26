//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_DRAWABLE_H
#define FEATHERGL_DRAWABLE_H

#include <vector>
#include "../materials/Material.h"
#include "Camera.h"

class Drawable {
public:
    explicit Drawable(const char* name);

    void setMaterial(Material *material);

    void setVertexData(std::vector<GLfloat> *vertices, std::vector<GLint> *indices, std::vector<GLfloat> *colors);

    void setPosition(glm::vec3 * newPosition);

    void setPositionFromFloats(float newX, float newY, float newZ);

    void render(Camera* camera);

private:
    const char* _name;
    std::vector<GLfloat> _vertices;
    std::vector<GLfloat> _colors;
    std::vector<GLint> _indices;
    glm::vec3 _position;
    Material _material;
    GLuint _vao;
    GLuint _vbo;
};


#endif //FEATHERGL_DRAWABLE_H
