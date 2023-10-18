//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_UTILS_H
#define FEATHERGL_UTILS_H

#include <string>
#include "glad/glad.h"

/**
 * Load file given by filename to the stringBuffer
 * @param filename
 * @param stringBuffer
 */
void loadFileToBuffer(const char *filename, std::string &stringBuffer);

GLuint loadTextureFromFileToGPU(const char *filename);

void writeTextureToPPM(GLuint textureHandle, const char *filename);

void writeDepthTextureToPPM(GLuint textureHandle, const char *filename);

#endif //FEATHERGL_UTILS_H
