#ifndef FEATHERGL_POSTPROCESSING_H
#define FEATHERGL_POSTPROCESSING_H

#include "glad/glad.h"

#include <cstring>
#include <fstream>
#include <sstream>
#include <iostream>
#include "utils.h"
#include "Shader.h"

class PostProcessing_Shader {
private:
    GLuint _program;

    void addShader(GLuint theProgram, const char *shaderCode, GLenum shaderType);

    void compile(const char *vShaderCode, const char *fShaderCode);

public:
    PostProcessing_Shader();

    // Create a _program program using vertex and fragment shaders
    void CreateProgram(std::string vShader, std::string fShader);

    // Using a _program program
    void bind() {
        glUseProgram(this->_program);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    GLuint program() { return this->_program; }

    ~PostProcessing_Shader();
};

class PostProcessing {
private:
    int width, height;

    unsigned int FBO;
    unsigned int framebufferTexture;

    PostProcessing_Shader *framebuffer_shader;

    GLuint uniformScreenResolution;

    static const float rectangleVerts[];

    unsigned int rectVAO;
public:
    PostProcessing(int _width, int _height, const char *vShader_Path, const char *fShader_Path);

    void StartProcessing();

    void EndProcessing();

    ~PostProcessing();
};

#endif //FEATHERGL_POSTPROCESSING_H