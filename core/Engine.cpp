//
// Created by barth on 25/09/23.
//

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include "Engine.h"

Engine::Engine(int windowWidth, int windowHeight, const char *name = "Feather Project") {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    // OpenGL 4.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(windowWidth, windowHeight, name, nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwSetErrorCallback([](int error, const char *description) {
        throw std::runtime_error(description);
    });

    if (glfwRawMouseMotionSupported()) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    onKeyPressObservable.add([this](int key) {
        if (key == GLFW_KEY_ESCAPE) {
            glfwSetWindowShouldClose(window, true); // Closes the application if the escape key is pressed
        }
    });

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize GLAD");
    }

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            engine->_keyStates[key] = true;
            engine->onKeyPressObservable.notifyObservers(key);
        } else if (action == GLFW_RELEASE) {
            engine->_keyStates[key] = false;
            engine->onKeyReleaseObservable.notifyObservers(key);
        }
    });

    glfwSetScrollCallback(window, [](GLFWwindow *window, double xOffset, double yOffset) {
        auto *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));
        engine->onMouseScrollObservable.notifyObservers(xOffset, yOffset);
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow *window, double xpos, double ypos) {
        auto *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));
        double mouseDX = xpos - engine->mouseX;
        double mouseDY = ypos - engine->mouseY;
        if (mouseDX != 0 || mouseDY != 0) {
            engine->onMouseMoveObservable.notifyObservers(mouseDX, mouseDY);
        }

        engine->mouseX = xpos;
        engine->mouseY = ypos;
    });

    glfwSetFramebufferSizeCallback(window, [](GLFWwindow *window, int width, int height) {
        glViewport(0, 0, width, height);

        auto *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));
        engine->onWindowResizeObservable.notifyObservers(width, height);
    });

    // blueish background
    setClearColor(0.4f, 0.6f, 0.6f, 1.0f);

    lastFrameTime = getElapsedSeconds();

    glDepthFunc(GL_LESS);   // Specify the depth test for the z-buffer
    glEnable(GL_DEPTH_TEST);      // Enable the z-buffer test in the rasterization
    glEnable(GL_BLEND);
    glCullFace(GL_BACK); // Specifies the faces to cull (here the ones pointing away from the camera)
}
