#include "postprocessing.h"

const float PostProcessing::rectangleVerts[] = {
        // Coords    // texCoords
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f,

        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f, 0.0f,
        -1.0f, 1.0f, 0.0f, 1.0f
};

PostProcessing::PostProcessing(const char *shaderFolder, Engine *engine) {
    engine->windowSize(&_width, &_height);

    // Prepare framebuffer rectangle VBO and VAO
    unsigned int rectVBO;
    glGenVertexArrays(1, &rectVAO);
    glGenBuffers(1, &rectVBO);
    glBindVertexArray(rectVAO);
    glBindBuffer(GL_ARRAY_BUFFER, rectVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(rectangleVerts), &rectangleVerts, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));

    // Create Frame Buffer Object
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Create Framebuffer Texture
    glGenTextures(1, &outputTexture);
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, outputTexture, 0);

    // Create Render Buffer Object
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    // Error checking framebuffer
    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: " << fboStatus << std::endl;

    _shader = new Shader(shaderFolder);

    // get screen resolution uniform locations
    uniformScreenResolution = glGetUniformLocation(_shader->program(), "screenResolution");
}

void PostProcessing::RenderTo(unsigned int targetFramebuffer) {
    // Bind the default framebuffer
    glBindFramebuffer(GL_FRAMEBUFFER, targetFramebuffer);
    // Draw the framebuffer rectangle
    _shader->bind();

    onBeforeRenderObservable.notifyObservers();

    // resize screen resolution uniform
    _shader->setVec2("screenResolution", (float) _width, (float) _height);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glBindVertexArray(rectVAO);
    glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}

void PostProcessing::resize(int width, int height) {
    _width = width;
    _height = height;

    // resize framebuffer texture
    glBindTexture(GL_TEXTURE_2D, outputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

    // resize render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);

    // Error checking framebuffer
    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: " << fboStatus << std::endl;
}