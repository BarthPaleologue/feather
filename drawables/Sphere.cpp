//
// Created by barth on 26/09/2022.
//

#include "Sphere.h"

Sphere::Sphere(const char *name, int resolution, float x, float y, float z): Drawable(name) {
    setPositionFromFloats(x, y, z);
    std::vector<GLfloat> positions;
    std::vector<GLint> indices;
    std::vector<GLfloat> colors;
    setVertexData(&positions, &indices, &colors);
}
