//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_ABSTRACTMESH_H
#define FEATHERGL_ABSTRACTMESH_H

#include <vector>
#include "../materials/Material.h"
#include "cameras/Camera.h"
#include "Transform.h"
#include "lights/PointLight.h"

class AbstractMesh: public Transform {
public:
    explicit AbstractMesh(const char* name);

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
    GLuint _vao;
    GLuint _vbo;
protected:
    Material *_material;
};


#endif //FEATHERGL_ABSTRACTMESH_H
