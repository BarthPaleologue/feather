#include "Scene.h"
#include "cameras/OrbitCamera.h"
#include "lights/PointLight.h"
#include "StandardMaterial.h"
#include "CelestialBody.h"
#include "Engine.h"
#include "MeshBuilder.h"
#include "DebugLight.h"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;

int main() {
    Engine engine(WINDOW_WIDTH, WINDOW_HEIGHT, "Solar System");

    Scene scene(&engine);

    OrbitCamera camera(&engine);
    camera.setPosition(0, 0, 20);
    camera.setRadius(20.0);
    camera.rotateTheta(-3.0f);
    scene.setActiveCamera(&camera);

    PointLight light("sun");
    scene.addPointLight(&light);

    PointLight light2("light2");
    light2.setColor(1, 0, 0);
    light2.transform()->setPosition(0.0f, 0.0f, 10.0f);
    DebugLight::CreateDebugPointLightMesh(&light2, scene);
    scene.addPointLight(&light2);

    PointLight light3("light3");
    light3.setColor(0, 1, 0);
    light3.transform()->setPosition(0.0f, 0.0f, -10.0f);
    DebugLight::CreateDebugPointLightMesh(&light3, scene);
    scene.addPointLight(&light3);

    PointLight light4("light4");
    light4.setColor(0, 0, 1);
    light4.transform()->setPosition(10.0f, 0.0f, 0.0f);
    DebugLight::CreateDebugPointLightMesh(&light4, scene);
    scene.addPointLight(&light4);

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

    CelestialBody sun("sun", 1, 100, 10, 0, &sunMaterial, scene);
    CelestialBody mercury("mercury", 0.2, 1.6, 1.5, 5, &mercuryMat, scene);
    CelestialBody venus("venus", 0.45, -0.1, 8, 7, &venusMat, scene);

    CelestialBody earth("earth", 0.5, 10, 10, 10, &earthMat, scene);
    earth.getMesh()->transform()->setRotationX(0.3);
    CelestialBody moon("moon", 0.25, 5, 5, 2, &moonMat, scene);

    CelestialBody mars("mars", 0.35, 10, 12, 13, &marsMat, scene);

    CelestialBody *currentTarget = &sun;

    Mesh *plane = MeshBuilder::makePlane("plane", scene, 64);
    plane->transform()->setPosition(0, -2, 0);
    plane->transform()->setScale(40);
    plane->material()->setWireframe(true);
    plane->material()->setBackFaceCullingEnabled(false);

    engine.onKeyPressObservable.add([&currentTarget, &sun, &earth, &moon, &plane](int key) {
        if (key == GLFW_KEY_S) {
            currentTarget = &sun;
        } else if (key == GLFW_KEY_E) {
            currentTarget = &earth;
        } else if (key == GLFW_KEY_M) {
            currentTarget = &moon;
        }

        if (key == GLFW_KEY_W) plane->material()->setWireframe(!plane->material()->wireframe());
    });

    scene.onBeforeRenderObservable.add([&]() {
        auto elapsedTime = engine.getElapsedTime();

        mercury.update(elapsedTime);
        venus.update(elapsedTime);
        earth.update(elapsedTime);
        moon.update(elapsedTime);
        moon.getMesh()->transform()->translate(*earth.getMesh()->transform()->position());
        mars.update(elapsedTime);

        light2.transform()->setPosition(
                10.0f * cosf(1.0f * elapsedTime), 10.0f * sinf(1.0f * elapsedTime), 0.0f
        );

        light3.transform()->setPosition(
                10.0f * cosf(1.0f * elapsedTime), 0.0f, 10.0f * sinf(1.0f * elapsedTime)
        );

        light4.transform()->setPosition(
                0.0f, 10.0f * cosf(1.0f * elapsedTime), 10.0f * sinf(1.0f * elapsedTime)
        );

        camera.setTarget(currentTarget->getMesh()->transform()->position());
        camera.setMinRadius(currentTarget->getRadius());

        camera.update();
    });

    engine.onExecuteLoopObservable.add([&scene] {
        scene.render();
    });

    engine.start();

    return 0;
}
