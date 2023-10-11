#include "Scene.h"
#include "cameras/OrbitCamera.h"
#include "lights/PointLight.h"
#include "CelestialBody.h"
#include "Engine.h"
#include "MeshLoader.h"
#include "DebugLight.h"
#include "physics/HpbdSolver.h"
#include "physics/Cloth.h"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;

int main() {
    Engine engine(WINDOW_WIDTH, WINDOW_HEIGHT, "Solar System");

    HpbdSolver solver;

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

    PhongMaterial sphereMaterial;
    sphereMaterial.setDiffuseColor(1.0, 1.0, 1.0);

    Mesh *sphere = MeshBuilder::makeSphere("sphere", scene, 32);
    sphere->transform()->setPosition(0, 5, 0);
    sphere->setMaterial(&sphereMaterial);
    sphere->material()->setWireframe(true);

    solver.addMesh(sphere, 1.0f);
    solver.applyForce(sphere, glm::vec3(0, -9.8, 0));

    PhysicsBody *cloth = new Cloth("cloth", scene, 32, 0.1f);

    cloth->transform()->setRotationX(3.14 / 2.0);
    cloth->mesh()->bakeTransformIntoVertexData();
    cloth->syncWithMesh();
    cloth->transform()->setScale(10);
    cloth->transform()->setPosition(0, 5, -6);

    cloth->mesh()->material()->setBackFaceCullingEnabled(false);
    cloth->mesh()->material()->setWireframe(true);

    solver.addBody(cloth);

    Mesh *plane = MeshBuilder::makePlane("plane", scene, 64);
    plane->transform()->setPosition(0, -2, 0);
    plane->transform()->setScale(40);
    plane->material()->setWireframe(true);
    plane->material()->setBackFaceCullingEnabled(false);

    engine.onKeyPressObservable.add([&](int key) {
        if (key == GLFW_KEY_W) plane->material()->setWireframe(!plane->material()->wireframe());
    });

    scene.onBeforeRenderObservable.add([&]() {
        float deltaTime = engine.getDeltaTime();

        solver.solve(deltaTime);

        camera.update();
    });

    engine.onExecuteLoopObservable.add([&scene] {
        scene.render();
    });

    engine.start();

    return 0;
}
