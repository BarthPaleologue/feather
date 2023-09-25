//
// Created by barth on 25/09/23.
//

#ifndef FEATHERGL_ENGINE_H
#define FEATHERGL_ENGINE_H


class Engine {
public:
    Engine(int windowWidth, int windowHeight, const char *name);

    void setCursorEnabled(bool enabled);

    GLFWwindow *getWindow() const;

    ~Engine();

private:
    GLFWwindow *window;
};


#endif //FEATHERGL_ENGINE_H
