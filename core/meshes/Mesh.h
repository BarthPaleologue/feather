//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_MESH_H
#define FEATHERGL_MESH_H

#include <vector>
#include "../materials/Material.h"
#include "cameras/Camera.h"
#include "Transform.h"
#include "lights/PointLight.h"
#include "VertexData.h"

class Mesh : public Transform {
public:
    explicit Mesh(const char *name);

    static Mesh *FromVertexData(const char *name, VertexData &vertexData);

    void setMaterial(Material *material);

    void setVertexData(VertexData &vertexData);

    void render(Camera &camera, PointLight &light);

private:
    const char *_name;
    VertexData _vertexData;
    GLuint _vao;
    GLuint _vbo;
protected:
    Material *_material;
};


#endif //FEATHERGL_MESH_H
