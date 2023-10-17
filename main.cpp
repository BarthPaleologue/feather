#include "Scene.h"
#include "cameras/OrbitCamera.h"
#include "lights/PointLight.h"
#include "CelestialBody.h"
#include "Engine.h"
#include "MeshLoader.h"
#include "physics/HpbdSolver.h"
#include "physics/Cloth.h"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;

int main() {
    Engine engine(WINDOW_WIDTH, WINDOW_HEIGHT, "Solar System");

    HpbdSolver solver;

    Scene scene(&engine);

    OrbitCamera camera(&engine);
    camera.setRadius(20.0);
    camera.setTarget(glm::vec3(0, 3, 0));
    camera.rotateTheta(-3.14 / 4.0);
    camera.rotatePhi(3.14 / 3.0);
    scene.setActiveCamera(&camera);

    DirectionalLight light("sun", -1.0, 1.0, 0.0);
    scene.addDirectionalLight(&light);

    PostProcessing colorCorrection("./assets/shaders/colorCorrection", &engine);
    colorCorrection.onBeforeRenderObservable.add([&]() {
        colorCorrection.shader()->setFloat("gamma", 1.0f / 2.2f);
        colorCorrection.shader()->setFloat("exposure", 1.0f);
        colorCorrection.shader()->setFloat("contrast", 1.0f);
        colorCorrection.shader()->setFloat("saturation", 1.0f);
        colorCorrection.shader()->setFloat("brightness", 0.0f);
    });

    scene.addPostProcess(&colorCorrection);

    PhysicsBody *cloth = new Cloth("cloth", scene, 16, 0.1f);

    cloth->transform()->setRotationZ(-3.14 / 2.0);
    cloth->transform()->setRotationZ(-3.14 / 2.0);
    cloth->bakeTransformIntoVertexData();
    cloth->transform()->setScale(10);
    cloth->transform()->setPosition(0, 5, 0);

    auto *clothMaterial = new PhongMaterial();
    clothMaterial->setAmbientColor(1.0, 0.2, 0.2);
    clothMaterial->setBackFaceCullingEnabled(false);
    clothMaterial->setWireframe(true);
    cloth->mesh()->setMaterial(clothMaterial);

    solver.addBody(cloth);
    solver.applyForce(cloth->mesh(), glm::vec3(-2, -9.81, 0));

    Mesh *ground = MeshBuilder::makePlane("ground", scene, 64);
    ground->transform()->setPosition(0, -2, 0);
    ground->transform()->setScale(40);

    auto *groundMaterial = new PhongMaterial();
    groundMaterial->setDiffuseColor(0.5, 0.5, 0.5);
    groundMaterial->setBackFaceCullingEnabled(false);
    ground->setMaterial(groundMaterial);

    bool realTimePhysics = false;

    engine.onKeyPressObservable.add([&](int key) {
        if (key == GLFW_KEY_W) ground->material()->setWireframe(!ground->material()->wireframe());
        if (key == GLFW_KEY_SPACE) realTimePhysics = !realTimePhysics;
        if (!realTimePhysics && key == GLFW_KEY_ENTER) solver.solve(1.0f / 60.0f);
    });

    scene.onBeforeRenderObservable.add([&]() {
        float deltaTime = engine.getDeltaTime();

        if (realTimePhysics) solver.solve(deltaTime);

        camera.update();
    });

    engine.onExecuteLoopObservable.add([&scene] {
        scene.render();
    });

    engine.start();

    return 0;
}
