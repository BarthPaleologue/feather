//
// Created by barth on 30/09/2022.
//

#ifndef FEATHERGL_TEXTURE_H
#define FEATHERGL_TEXTURE_H

#include "../utils/utils.h"

class Texture {
public:
    explicit Texture(const char *filepath);

    explicit Texture(int id) : _id(id) {};

    void bind(int texShaderId);

    void unbind();

    int getId();

private:
    int _id;
};


#endif //FEATHERGL_TEXTURE_H
