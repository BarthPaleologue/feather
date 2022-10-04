#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "core/Scene.h"
#include "cameras/FreeCamera.h"
#include "Cube.h"
#include "cameras/OrbitCamera.h"
#include "Sphere.h"
#include "lights/PointLight.h"
#include "StandardMaterial.h"

#define AZERTY_KEY_Z GLFW_KEY_W
#define AZERTY_KEY_W GLFW_KEY_Z

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == AZERTY_KEY_W && action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (action == GLFW_PRESS && (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)) {
        glfwSetWindowShouldClose(window, true); // Closes the application if the escape key is pressed
    }
}

void errorCallback(int error, const char *desc) {
    std::cout << "Error " << error << ": " << desc << std::endl;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    std::cout << "Scrolling" << std::endl;
}


int main() {
    if (!glfwInit()) return -1;
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World !", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Scene scene;
    OrbitCamera camera(window);
    PointLight light("sun");

    StandardMaterial troncheMaterial;
    Texture texture("assets/textures/tronche.jpg");
    troncheMaterial.setDiffuseTexture(&texture);

    StandardMaterial sunMaterial;
    sunMaterial.setEmissiveColor(1.0, 1.0, 0.0);

    Cube sun("sun", 0.0f, 0.0f, 0.0f);
    sun.setMaterial(&sunMaterial);
    scene.addDrawable(sun);

    StandardMaterial earthMat;
    Texture earthMap("./assets/textures/earth.jpg");
    earthMat.setDiffuseTexture(&earthMap);

    Sphere earth("earth", 1, 32);
    earth.setMaterial(&earthMat);
    scene.addDrawable(earth);

    StandardMaterial moonMat;
    Texture moonMap("./assets/textures/moon.jpg");
    moonMat.setDiffuseTexture(&moonMap);

    Sphere moon("moon", 0.5, 32);
    moon.setParent(&earth);
    moon.setMaterial(&moonMat);
    scene.addDrawable(moon);


    glCullFace(GL_BACK); // Specifies the faces to cull (here the ones pointing away from the camera)
    glEnable(GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
    glDepthFunc(GL_LESS);   // Specify the depth test for the z-buffer
    glEnable(GL_DEPTH_TEST);      // Enable the z-buffer test in the rasterization
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        camera.update();
        auto time = (float) glfwGetTime();

        earth.setPositionFromFloats(7.0f * std::cos(time), 0, 7.0f * std::sin(time));
        //earth.setRotationY(time);

        moon.setPositionFromFloats(2.0f * std::cos(5.0f * time), 0, 2.0f * std::sin(5.0f * time));

        scene.render(camera, light);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
