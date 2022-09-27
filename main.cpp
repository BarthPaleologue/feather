#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "core/Scene.h"
#include "drawables/Triangle.h"
#include "cameras/FreeCamera.h"
#include "Cube.h"
#include "cameras/OrbitCamera.h"

#define AZERTY_KEY_Z GLFW_KEY_W
#define AZERTY_KEY_W GLFW_KEY_Z


void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(key == AZERTY_KEY_W && action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if(key == GLFW_KEY_F && action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if(action == GLFW_PRESS && (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)) {
        glfwSetWindowShouldClose(window, true); // Closes the application if the escape key is pressed
    }
}

void errorCallback(int error, const char *desc) {
    std::cout <<  "Error " << error << ": " << desc << std::endl;
}


int main() {
    if (!glfwInit()) return -1;
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
    OrbitCamera camera(window);

    Cube cube("cube", 1, 0.0f, 0.0f, 0.0f);
    scene.addDrawable(&cube);

    /*Triangle triangle("triangle", 0.2f, 0.0f, 0.0f);
    scene.addDrawable(&triangle);

    Triangle triangle2("triangle2", 0.5f, 0.3f, 2.0f);
    scene.addDrawable(&triangle2);*/

    //glCullFace(GL_BACK); // Specifies the faces to cull (here the ones pointing away from the camera)
    //glEnable(GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
    glDepthFunc(GL_LESS);   // Specify the depth test for the z-buffer
    glEnable(GL_DEPTH_TEST);      // Enable the z-buffer test in the rasterization
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        camera.update();
        scene.render(&camera);

        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
