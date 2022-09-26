#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include "Drawable.h"
#include "Scene.h"


void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS && (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)) {
        // closes the application if the escape or 'Q' key is pressed
        glfwSetWindowShouldClose(window, true);
    }
}


int main() {
    if (glfwInit() == GLFW_FALSE) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World !", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Scene scene;

    Drawable triangle("triangle");
    scene.addDrawable(&triangle);
    std::vector<GLfloat> points = {
            0.0f, 0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f
    };
    triangle.setVertexData(&points);

    Drawable triangle2("triangle2");
    scene.addDrawable(&triangle2);
    std::vector<float> points2 = {
            0.4f, 0.7f, 0.0f,
            0.2f, 0.7f, 0.0f,
            0.2f, 0.5f, 0.0f
    };
    triangle2.setVertexData(&points2);

    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        scene.render();

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
