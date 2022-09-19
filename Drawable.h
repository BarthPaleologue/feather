//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_DRAWABLE_H
#define FEATHERGL_DRAWABLE_H

#include <vector>
#include "Material.h"

class Drawable {
public:
    explicit Drawable(const char* name);

    void setMaterial(Material *material);

    void setVertexData(std::vector<float> *vertices);

    void render();

private:
    const char* _name;
    std::vector<float> _vertices;
    Material _material;
    GLuint _vao;
    GLuint _vbo;
};


#endif //FEATHERGL_DRAWABLE_H
