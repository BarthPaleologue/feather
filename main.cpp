#include "Scene.h"
#include "cameras/OrbitCamera.h"
#include "lights/PointLight.h"
#include "CelestialBody.h"
#include "Engine.h"
#include "MeshLoader.h"
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
    light.transform()->setPosition(50, 50, 0);
    scene.addPointLight(&light);

    PostProcessing colorCorrection("./assets/shaders/colorCorrection", &engine);
    colorCorrection.onBeforeRenderObservable.add([&]() {
        colorCorrection.shader()->setFloat("gamma", 1.0f / 2.2f);
        colorCorrection.shader()->setFloat("exposure", 1.0f);
        colorCorrection.shader()->setFloat("contrast", 1.0f);
        colorCorrection.shader()->setFloat("saturation", 1.0f);
        colorCorrection.shader()->setFloat("brightness", 0.0f);
    });

    scene.addPostProcess(&colorCorrection);

    PhongMaterial sunMaterial;
    sunMaterial.setDiffuseColor(1.0, 1.0, 1.0);

    Mesh *sphere = MeshBuilder::makeSphere("sun", scene, 32);
    sphere->transform()->setPosition(0, 5, 0);
    sphere->setMaterial(&sunMaterial);
    sphere->material()->setWireframe(true);

    Mesh *plane = MeshBuilder::makePlane("plane", scene, 64);
    plane->transform()->setPosition(0, -2, 0);
    plane->transform()->setScale(40);
    plane->material()->setWireframe(true);
    plane->material()->setBackFaceCullingEnabled(false);

    engine.onKeyPressObservable.add([&](int key) {
        if (key == GLFW_KEY_W) plane->material()->setWireframe(!plane->material()->wireframe());
    });

    scene.onBeforeRenderObservable.add([&]() {
        auto elapsedTime = engine.getElapsedTime();

        camera.update();
    });

    engine.onExecuteLoopObservable.add([&scene] {
        scene.render();
    });

    engine.start();

    return 0;
}
