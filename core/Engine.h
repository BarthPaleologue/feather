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

    void setCursorEnabled(bool enabled) {
        glfwSetInputMode(window, GLFW_CURSOR, enabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
    }

    void setWireframeEnabled(bool enabled) {
        _isWireframeEnabled = enabled;
        glPolygonMode(GL_FRONT_AND_BACK, enabled ? GL_LINE : GL_FILL);
    }

    bool isWireframeEnabled() const {
        return _isWireframeEnabled;
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

    float getElapsedTime() const {
        return (float) glfwGetTime();
    }

    ~Engine();

    Observable<int> onKeyPressObservable{};
    Observable<int> onKeyReleaseObservable{};
    Observable<double, double> onMouseScrollObservable{};
    Observable<double, double> onMouseMoveObservable{};

private:
    double mouseX{};
    double mouseY{};

    std::map<int, bool> _keyStates{};

    bool _isWireframeEnabled = false;
    GLFWwindow *window;
};


#endif //FEATHERGL_ENGINE_H
