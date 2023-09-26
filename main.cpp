#include "Scene.h"
#include "cameras/OrbitCamera.h"
#include "lights/PointLight.h"
#include "StandardMaterial.h"
#include "CelestialBody.h"
#include "Engine.h"
#include "FreeCamera.h"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;

int main() {
    Engine engine(WINDOW_WIDTH, WINDOW_HEIGHT, "Solar System");
    engine.onKeyPressObservable.add([&engine](int key) {
        if (key == GLFW_KEY_W) engine.setWireframeEnabled(!engine.isWireframeEnabled());
    });

    Scene scene;
    OrbitCamera camera(&engine);
    camera.setPosition(0, 0, 20);
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

    engine.onKeyPressObservable.add([&currentTarget, &sun, &earth, &moon](int key) {
        if (key == GLFW_KEY_S) {
            currentTarget = &sun;
        } else if (key == GLFW_KEY_E) {
            currentTarget = &earth;
        } else if (key == GLFW_KEY_M) {
            currentTarget = &moon;
        }
    });

    scene.onBeforeRenderObservable.add([&]() {
        auto elapsedTime = engine.getElapsedTime();

        mercury.update(elapsedTime);
        venus.update(elapsedTime);
        earth.update(elapsedTime);
        moon.update(elapsedTime);
        moon.translate(*earth.getPosition());
        mars.update(elapsedTime);

        camera.setTarget(currentTarget->getPosition());
        camera.setMinRadius(currentTarget->getRadius());

        camera.update();
    });

    engine.onExecuteLoopObservable.add([&scene, &light] {
        scene.render(light);
    });

    engine.start();

    return 0;
}
