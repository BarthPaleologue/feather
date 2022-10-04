//
// Created by barth on 26/09/2022.
//

#include "Sphere.h"

Sphere::Sphere(const char *name, float radius, int resolution) : Drawable(name), _radius(radius) {
    setScale(radius);

    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> uvs;
    std::vector<GLint> indices;
    std::vector<GLfloat> colors;

    const float PI = std::acos(-1.0f);

    float x, y, z, xy;                              // vertex position
    float nx, ny, nz, lengthInv = 1.0f;    // normal
    float s, t;                                     // texCoord

    float sectorStep = 2 * PI / (float) resolution;
    float stackStep = PI / (float) resolution;
    float sectorAngle, stackAngle;

    for (int i = 0; i <= resolution; ++i) {
        stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
        xy = radius * cosf(stackAngle);             // r * cos(u)
        z = radius * sinf(stackAngle);              // r * sin(u)

        // add (sectorCount+1) vertices per stack
        // the first and last vertices have same position and normal, but different tex coords
        for (int j = 0; j <= resolution; ++j) {
            sectorAngle = j * sectorStep;           // starting from 0 to 2pi

            // vertex position
            x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
            y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
            vertices.push_back(x);
            vertices.push_back(z);
            vertices.push_back(y);

            // normalized vertex normal
            nx = x * lengthInv;
            ny = z * lengthInv;
            nz = y * lengthInv;
            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            // vertex tex coord between [0, 1]
            s = (float) j / (float) resolution;
            t = (float) i / (float) resolution;
            uvs.push_back(1.0f - s);
            uvs.push_back(t);
        }
    }

    // indices
    //  k1--k1+1
    //  |  / |
    //  | /  |
    //  k2--k2+1
    unsigned int k1, k2;
    for (int i = 0; i < resolution; ++i) {
        k1 = i * (resolution + 1);     // beginning of current stack
        k2 = k1 + resolution + 1;      // beginning of next stack

        for (int j = 0; j < resolution; ++j, ++k1, ++k2) {
            // 2 triangles per sector excluding 1st and last stacks
            if (i != 0) {
                indices.push_back(k1);
                //indices.push_back(k2);
                indices.push_back(k1 + 1);   // k1---k2---k1+1
                indices.push_back(k2);
            }

            if (i != (resolution - 1)) {
                indices.push_back(k1 + 1);
                //indices.push_back(k2);
                indices.push_back(k2 + 1); // k1+1---k2---k2+1
                indices.push_back(k2);
            }
        }
    }

    for (int i = 0; i < vertices.size(); i++) colors.push_back((float) random() / (float) INT_MAX);

    setVertexData(&vertices, &indices, &normals, &uvs, &colors);
}
