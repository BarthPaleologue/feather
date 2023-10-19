//
// Created by barth on 25/09/23.
//

#ifndef FEATHERGL_ENGINE_H
#define FEATHERGL_ENGINE_H

#include "Observable.h"
#include <GLFW/glfw3.h>
#include <map>

class Engine {
public:
    Engine(int windowWidth, int windowHeight, const char *name);

    ~Engine() {
        glfwDestroyWindow(window);
    }

    void setCursorEnabled(bool enabled) {
        glfwSetInputMode(window, GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    bool isMousePressed() const {
        return glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
    }

    bool isKeyPressed(int key) const {
        return _keyStates.find(key) != _keyStates.end() && _keyStates.at(key);
    }

    GLFWwindow *getWindow() const {
        return window;
    }

    void windowSize(int *width, int *height) {
        glfwGetWindowSize(window, width, height);
    }

    float getElapsedTime() const {
        return (float) glfwGetTime();
    }

    float getDeltaTime() const {
        return deltaTime;
    }

    void setClearColor(float r, float g, float b, float a) {
        glClearColor(r, g, b, a);
    }

    void start() {
        int width, height;
        glfwGetWindowSize(window, &width, &height);
        onWindowResizeObservable.notifyObservers(width, height);

        while (!glfwWindowShouldClose(window)) {
            float newFrameTime = getElapsedTime();
            deltaTime = newFrameTime - lastFrameTime;
            lastFrameTime = newFrameTime;

            onExecuteLoopObservable.notifyObservers();
            glfwPollEvents();
            glfwSwapBuffers(window);
        }

        glfwTerminate();
    }

    Observable<> onExecuteLoopObservable{};

    Observable<int, int> onWindowResizeObservable{};

    Observable<int> onKeyPressObservable{};
    Observable<int> onKeyReleaseObservable{};
    Observable<double, double> onMouseScrollObservable{};
    Observable<double, double> onMouseMoveObservable{};

private:
    double mouseX{};
    double mouseY{};

    float lastFrameTime{};
    float deltaTime{};

    std::map<int, bool> _keyStates{};

    GLFWwindow *window;
};


#endif //FEATHERGL_ENGINE_H
