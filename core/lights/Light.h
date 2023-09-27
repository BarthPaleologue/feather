//
// Created by barth on 27/09/2022.
//

#ifndef FEATHERGL_LIGHT_H
#define FEATHERGL_LIGHT_H


#include <string>
#include "glm/ext.hpp"

class Light {
public:
    explicit Light(const char *name);

    inline void setColor(float r, float g, float b) {
        _color->x = r;
        _color->y = g;
        _color->z = b;
    }

    inline glm::vec3 *color() { return _color; };

    float intensity() const { return _intensity; };

private:
    std::string _name;

    float _intensity = 1.0f;

    glm::vec3 *_color = new glm::vec3(1.0f);
};


#endif //FEATHERGL_LIGHT_H
