//
// Created by barth on 25/09/23.
//

#include <GLFW/glfw3.h>
#include <stdexcept>
#include "Engine.h"

Engine::Engine(int windowWidth, int windowHeight, const char *name = "Feather Project") {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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

    glfwSetWindowUserPointer(window, this);
    glfwSetKeyCallback(window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        auto *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));
        if (action == GLFW_PRESS) {
            engine->onKeyPressObservable.notifyObservers(key);
        }
    });

    glfwSetScrollCallback(window, [](GLFWwindow *window, double xOffset, double yOffset) {
        auto *engine = static_cast<Engine *>(glfwGetWindowUserPointer(window));
        engine->onMouseScrollObservable.notifyObservers(xOffset, yOffset);
    });

}

void Engine::setCursorEnabled(bool enabled) {
    glfwSetInputMode(window, GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
}

GLFWwindow *Engine::getWindow() const {
    return window;
}

Engine::~Engine() = default;
