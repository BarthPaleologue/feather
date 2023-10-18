//
// Created by barth on 19/09/2022.
//

#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <glm/vec4.hpp>
#include "utils.h"

#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"

void loadFileToBuffer(const char *filename, std::string &stringBuffer) {
    stringBuffer = ""; // empty buffer first
    std::ifstream reader(filename);
    if (reader.is_open()) {
        std::string lineBuffer;
        while (std::getline(reader, lineBuffer)) {
            stringBuffer += lineBuffer + "\n";
        }
        reader.close();
    } else std::cerr << "Cloud not open " << filename << std::endl;
}

GLuint loadTextureFromFileToGPU(const char *filename) {
    int width, height, numComponents;
    // Loading the image in CPU memory using stb_image
    unsigned char *data = stbi_load(
            filename,
            &width, &height,
            &numComponents, // 1 for a 8 bit grey-scale image, 3 for 24bits RGB image, 4 for 32bits RGBA image
            0);

    GLuint texID;
    glGenTextures(1, &texID); // generate an OpenGL texture container
    glBindTexture(GL_TEXTURE_2D, texID); // activate the texture
// Setup the texture filtering option and repeat mode; check www.opengl.org for details.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
// Fill the GPU texture with the data stored in the CPU image
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // Free useless CPU memory
    stbi_image_free(data);
    //glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture

    return texID;
}

void writeTextureToPPM(GLuint textureHandle, const char *filename) {
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    int width, height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    std::vector<glm::vec4> pixels(width * height);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixels.data());

    std::ofstream ofs(filename, std::ios::out | std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (int i = 0; i < width * height; ++i) {
        unsigned char r = static_cast<unsigned char>(pixels[i].r * 255);
        unsigned char g = static_cast<unsigned char>(pixels[i].g * 255);
        unsigned char b = static_cast<unsigned char>(pixels[i].b * 255);
        ofs << r << g << b;
    }
    ofs.close();
}

void writeDepthTextureToPPM(GLuint textureHandle, const char *filename) {
    glBindTexture(GL_TEXTURE_2D, textureHandle);
    int width, height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
    std::vector<float> pixels(width * height);
    glGetTexImage(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, GL_FLOAT, pixels.data());

    std::ofstream ofs(filename, std::ios::out | std::ios::binary);
    ofs << "P5\n" << width << " " << height << "\n65535\n";
    for (int i = 0; i < width * height; ++i) {
        unsigned short depth = static_cast<unsigned short>(pixels[i] * 65535);
        ofs.write((char *) &depth, sizeof(unsigned short));
    }
    ofs.close();
}