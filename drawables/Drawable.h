//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_DRAWABLE_H
#define FEATHERGL_DRAWABLE_H

#include <vector>
#include "../materials/Material.h"
#include "cameras/Camera.h"
#include "Transform.h"
#include "lights/PointLight.h"

class Drawable: public Transform {
public:
    explicit Drawable(const char* name);

    void setMaterial(Material *material);

    void setVertexData(std::vector<GLfloat> *vertices, std::vector<GLint> *indices, std::vector<GLfloat> *normals, std::vector<GLfloat> *uvs, std::vector<GLfloat> *colors);

    void render(Camera &camera, PointLight &light);

private:
    const char* _name;
    std::vector<GLfloat> _vertices;
    std::vector<GLfloat> _normals;
    std::vector<GLfloat> _uvs;
    std::vector<GLfloat> _colors;
    std::vector<GLint> _indices;
    Material _material;
    Texture* _texture;
    GLuint _vao;
    GLuint _vbo;
};


#endif //FEATHERGL_DRAWABLE_H
