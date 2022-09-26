//
// Created by barth on 19/09/2022.
//

#define STB_IMAGE_IMPLEMENTATION
#include <string>
#include <fstream>
#include <iostream>
#include "utils.h"
#include "glad/glad.h"
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

/*GLuint loadTextureFromFileToGPU(const std::string &filename) {
    int width, height, numComponents;
    // Loading the image in CPU memory using stb_image
    unsigned char *data = stbi_load(
            filename.c_str(),
            &width, &height,
            &numComponents, // 1 for a 8 bit grey-scale image, 3 for 24bits RGB image, 4 for 32bits RGBA image
            0);

    GLuint texID;
    // TODO: create a texture and upload the image data in GPU memory using
    // glGenTextures, glBindTexture, glTexParameteri, and glTexImage2D

    // Free useless CPU memory
    stbi_image_free(data);
    glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture

    return texID;
}*/