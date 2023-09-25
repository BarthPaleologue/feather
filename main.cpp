#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "core/Scene.h"
#include "cameras/OrbitCamera.h"
#include "lights/PointLight.h"
#include "StandardMaterial.h"
#include "CelestialBody.h"
#include "Engine.h"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;

double scrollOffset = 0.0;
double mouseX = -1.0;
double mouseY = -1.0;
double mouseDX = 0.0;
double mouseDY = 0.0;

void cursor_position_callback(GLFWwindow *window, double xpos, double ypos) {
    mouseDX = xpos - mouseX;
    mouseDY = ypos - mouseY;
    if (mouseX == -1) mouseDX = 0;
    if (mouseY == -1) mouseDY = 0;
    if (std::abs(mouseDX) < 7.0) mouseDX = 0;
    if (std::abs(mouseDY) < 7.0) mouseDY = 0;

    mouseX = xpos;
    mouseY = ypos;
}

int main() {
    Engine engine(WINDOW_WIDTH, WINDOW_HEIGHT, "Hello World !");
    GLFWwindow *window = engine.getWindow();

    engine.onKeyPressObservable.add([](int key) {
        std::cout << "Key pressed: " << key << std::endl;

        if (key == GLFW_KEY_W) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        } else if (key == GLFW_KEY_F) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        }
    });

    engine.onMouseScrollObservable.add([](double xOffset, double yOffset) {
        scrollOffset = yOffset / 5.0;
    });

    glfwSetCursorPosCallback(window, cursor_position_callback);

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Scene scene;
    OrbitCamera camera(window);
    camera.setRadius(20.0);
    camera.rotateTheta(-3.0f);
    scene.setActiveCamera(&camera);
    PointLight light("sun");

    StandardMaterial sunMaterial;
    Texture sunMap("assets/textures/sun.jpg");
    sunMaterial.setAmbientTexture(&sunMap);

    StandardMaterial mercuryMat;
    mercuryMat.setDiffuseTextureFromFile("./assets/textures/mercury.jpg");

    StandardMaterial venusMat;
    venusMat.setDiffuseTextureFromFile("./assets/textures/venus.jpg");

    StandardMaterial earthMat;
    earthMat.setDiffuseTextureFromFile("./assets/textures/earth.jpg");
    earthMat.setAmbientColor(0, 0.02, 0);

    StandardMaterial moonMat;
    moonMat.setDiffuseTextureFromFile("./assets/textures/moon.jpg");
    moonMat.setAmbientColor(0, 0, 0.02);

    StandardMaterial marsMat;
    marsMat.setDiffuseTextureFromFile("./assets/textures/mars.jpg");

    CelestialBody sun("sun", 1, 100, 10, 0, &sunMaterial);
    scene.addDrawable(sun);

    CelestialBody mercury("mercury", 0.2, 1.6, 1.5, 5, &mercuryMat);
    scene.addDrawable(mercury);

    CelestialBody venus("venus", 0.45, -0.1, 8, 7, &venusMat);
    scene.addDrawable(venus);

    CelestialBody earth("earth", 0.5, 10, 10, 10, &earthMat);
    earth.setRotationX(0.3);
    scene.addDrawable(earth);

    CelestialBody moon("moon", 0.25, 5, 5, 2, &moonMat);
    moon.setMaterial(&moonMat);
    scene.addDrawable(moon);

    CelestialBody mars("mars", 0.35, 10, 12, 13, &marsMat);
    scene.addDrawable(mars);


    glCullFace(GL_BACK); // Specifies the faces to cull (here the ones pointing away from the camera)
    glEnable(GL_CULL_FACE); // Enables face culling (based on the orientation defined by the CW/CCW enumeration).
    glDepthFunc(GL_LESS);   // Specify the depth test for the z-buffer
    glEnable(GL_DEPTH_TEST);      // Enable the z-buffer test in the rasterization
    glEnable(GL_BLEND);
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

    CelestialBody *currentTarget = &sun;

    while (!glfwWindowShouldClose(window)) {
        auto elapsedTime = engine.getElapsedTime();

        mercury.update(elapsedTime);
        venus.update(elapsedTime);
        earth.update(elapsedTime);
        moon.update(elapsedTime);
        moon.translate(*earth.getPosition());
        mars.update(elapsedTime);

        if (glfwGetKey(window, GLFW_KEY_S)) {
            currentTarget = &sun;
        } else if (glfwGetKey(window, GLFW_KEY_E)) {
            currentTarget = &earth;
        } else if (glfwGetKey(window, GLFW_KEY_M)) {
            currentTarget = &moon;
        }

        camera.setTarget(currentTarget->getPosition());
        camera.setMinRadius(currentTarget->getRadius());
        camera.zoom((float) scrollOffset);
        scrollOffset = 0.0;
        camera.rotatePhi(-(float) mouseDX / 500.0f);
        camera.rotateTheta(-(float) mouseDY / 500.0f);
        camera.update();
        scene.render(light);
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    glfwTerminate();
    return 0;
}
