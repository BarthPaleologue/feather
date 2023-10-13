//
// Created by barth on 30/09/2022.
//

#ifndef FEATHERGL_TEXTURE_H
#define FEATHERGL_TEXTURE_H

#include "../utils/utils.h"

class Texture {
public:
    explicit Texture(const char *filepath);

    explicit Texture(int id) : _handle(id) {};

    ~Texture() {
        glDeleteTextures(1, &_handle);
    }

    Texture(unsigned int width, unsigned int height) {
        glGenTextures(1, &_handle);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, _handle);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, (int) width, (int) height, 0, GL_RGBA, GL_FLOAT, nullptr);
        glBindImageTexture(0, _handle, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    }

    void bind(int texShaderId);

    void unbind();

    unsigned int handle() const {
        return _handle;
    }

    unsigned int &handlePtr() {
        return _handle;
    }

private:
    unsigned int _handle{};
};


#endif //FEATHERGL_TEXTURE_H
