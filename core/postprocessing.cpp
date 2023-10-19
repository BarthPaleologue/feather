#include "postprocessing.h"
#include "meshes/MeshBuilder.h"

PostProcessing::PostProcessing(const char *shaderFolder, Engine *engine) {
    engine->windowSize(&_width, &_height);

    screenQuad = std::make_shared<ScreenQuad>();

    // Create Frame Buffer Object
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Create Framebuffer Texture
    glGenTextures(1, &inputTexture);
    glBindTexture(GL_TEXTURE_2D, inputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // Prevents edge bleeding
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE); // Prevents edge bleeding

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, inputTexture, 0);

    // Create Render Buffer Object
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    // Error checking framebuffer
    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: " << fboStatus << std::endl;

    _shader = std::make_shared<Shader>(shaderFolder);

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
    glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
    auto projectionView = glm::identity<glm::mat4>();
    screenQuad->render();

    glBindVertexArray(0);
    glEnable(GL_DEPTH_TEST);
}

void PostProcessing::resize(int width, int height) {
    _width = width;
    _height = height;

    // resize framebuffer texture
    glBindTexture(GL_TEXTURE_2D, inputTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, _width, _height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);

    // resize render buffer
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, _width, _height);

    // Error checking framebuffer
    auto fboStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (fboStatus != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer error: " << fboStatus << std::endl;
}