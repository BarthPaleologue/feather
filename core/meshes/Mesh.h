//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_MESH_H
#define FEATHERGL_MESH_H

#include <vector>
#include "../materials/Material.h"
#include "cameras/Camera.h"
#include "Transformable.h"
#include "lights/PointLight.h"
#include "VertexData.h"
#include "DefaultMaterial.h"
#include "../utils/Uuid.h"

class Mesh : public Transformable {
public:
    explicit Mesh(const char *name) : Transformable(), _name(name), _vao(0), _vbo(0) {
        _id = UUID::generate_uuid_v4();
        _material = new DefaultMaterial();
        std::cout << "Mesh " << _name << " _handle: " << _id << " created" << std::endl;
    }

    bool operator==(const Mesh &other) const {
        return _id == other._id;
    }

    static Mesh *FromVertexData(const char *name, VertexData &vertexData);

    void setMaterial(Material *material);

    Material *material() const {
        return _material;
    }

    void bakeTransformIntoVertexData();

    void setVertexData(VertexData &vertexData);

    void updateVertexData();

    VertexData &vertexData() {
        return _vertexData;
    }

    void render(Camera *camera, std::vector<PointLight *> &lights);

private:
    const char *_name;
    std::string _id;
    VertexData _vertexData;

    GLuint _vao;
    GLuint _vbo;
    GLuint _ibo;
    GLuint _normalVbo;
    GLuint _uvVbo;
    GLuint _colVbo;
protected:
    Material *_material;
};


#endif //FEATHERGL_MESH_H
