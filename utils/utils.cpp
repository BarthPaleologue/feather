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
    glBindTexture(GL_TEXTURE_2D, 0); // unbind the texture

    return texID;
}