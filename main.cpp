#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "core/Scene.h"
#include "Cube.h"
#include "cameras/OrbitCamera.h"
#include "Sphere.h"
#include "lights/PointLight.h"
#include "StandardMaterial.h"

#define AZERTY_KEY_Z GLFW_KEY_W
#define AZERTY_KEY_W GLFW_KEY_Z

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;

double scrollOffset = 0.0;
double mouseX = 0.0;
double mouseY = 0.0;
double mouseDX = 0.0;
double mouseDY = 0.0;

void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if (key == AZERTY_KEY_W && action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    } else if (key == GLFW_KEY_F && action == GLFW_PRESS) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    } else if (action == GLFW_PRESS && (key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q)) {
        glfwSetWindowShouldClose(window, true); // Closes the application if the escape key is pressed
    }
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    mouseDX = xpos - mouseX;
    mouseDY = ypos - mouseY;
    if(std::abs(mouseDX) < 0.1) mouseDX = 0;
    if(std::abs(mouseDY) < 0.1) mouseDY = 0;

    mouseX = xpos;
    mouseY = ypos;
}

void errorCallback(int error, const char *desc) {
    std::cout << "Error " << error << ": " << desc << std::endl;
}

void scroll_callback(GLFWwindow *window, double xoffset, double yoffset) {
    scrollOffset = yoffset / 5.0;
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

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    if (glfwRawMouseMotionSupported()) glfwSetInputMode(window, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetCursorPosCallback(window, cursor_position_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Scene scene;
    OrbitCamera camera(window);
    camera.setRadius(1.0);
    PointLight light("sun");

    StandardMaterial troncheMaterial;
    Texture texture("assets/textures/tronche.jpg");
    troncheMaterial.setDiffuseTexture(&texture);

    StandardMaterial sunMaterial;
    sunMaterial.setAmbientColor(1.0, 1.0, 0.0);

    Cube sun("sun", 0.0f, 0.0f, 0.0f);
    sun.setMaterial(&sunMaterial);
    scene.addDrawable(sun);

    StandardMaterial earthMat;
    Texture earthMap("./assets/textures/earth.jpg");
    earthMat.setDiffuseTexture(&earthMap);
    earthMat.setAmbientColor(0, 0.02, 0);

    StandardMaterial cloudMat;
    Texture cloudMap("./assets/textures/clouds.jpg");
    cloudMat.setDiffuseTexture(&cloudMap);
    cloudMat.setAlphaColor(0, 0, 0);

    Sphere earth("earth", 0.5, 32);
    earth.setRotationX(0.3);
    earth.setMaterial(&earthMat);
    scene.addDrawable(earth);

    StandardMaterial moonMat;
    Texture moonMap("./assets/textures/moon.jpg");
    moonMat.setDiffuseTexture(&moonMap);
    moonMat.setAlphaColor(0, 0, 0.02);

    Sphere moon("moon", 0.25, 32);
    moon.setMaterial(&moonMat);
    scene.addDrawable(moon);


    glCullFace(GL_BACK); // Specifies the faces to cull (here the ones pointing away from the camera)
    glEnable(GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
    glDepthFunc(GL_LESS);   // Specify the depth test for the z-buffer
    glEnable(GL_DEPTH_TEST);      // Enable the z-buffer test in the rasterization
    glEnable(GL_BLEND);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    while (!glfwWindowShouldClose(window)) {
        auto time = (float) glfwGetTime();

        earth.setPositionFromFloats(10.0f * std::cos(time * 0.2f), 0, 10.0f * std::sin(time * 0.2f));
        earth.setRotationY(time * 0.4f);

        camera.setTarget(earth.getPosition());

        moon.setPositionFromFloats(2.0f * std::cos(time * 0.4f), 0, 2.0f * std::sin(time * 0.4f));
        moon.translate(*earth.getPosition());
        //moon.setRotationY(time * 0.4f);

        camera.zoom((float) scrollOffset);
        scrollOffset = 0.0;

        camera.rotatePhi(-(float)mouseDX / 500.0f);
        camera.rotateTheta(-(float)mouseDY / 500.0f);
        camera.update();
        scene.render(camera, light);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
