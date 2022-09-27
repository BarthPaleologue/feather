//
// Created by barth on 27/09/2022.
//

#include "Cube.h"

Cube::Cube(const char *name, int size, float x, float y, float z): Drawable(name) {
    setPositionFromFloats(x, y, z);
    std::vector<float> vertices = {
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f
    };
    std::vector<float> uvs = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
    };
}
