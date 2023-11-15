#include "Scene.h"
#include "cameras/OrbitCamera.h"
#include "lights/PointLight.h"
#include "CelestialBody.h"
#include "Engine.h"
#include "MeshLoader.h"
#include "physics/HpbdSolver.h"
#include "physics/Cloth.h"
#include "PbrMaterial.h"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;

int main() {
    Engine engine(WINDOW_WIDTH, WINDOW_HEIGHT, "HPBD Cloth Simulation");

    HpbdSolver solver;

    Scene scene((std::shared_ptr<Engine>(&engine)));

    OrbitCamera camera(&engine);
    camera.setRadius(20.0);
    camera.setTarget(glm::vec3(0, 3, 0));
    camera.rotateTheta(-3.14 / 4.0);
    camera.rotatePhi(3.14 / 3.0);
    scene.setActiveCamera(std::shared_ptr<Camera>(&camera));

    DirectionalLight light("sun", -1.0, 1.0, 0.0);
    scene.addDirectionalLight(std::shared_ptr<DirectionalLight>(&light));

    auto shadowRenderer = std::make_shared<ShadowRenderer>(std::shared_ptr<DirectionalLight>(&light));
    scene.addShadowRenderer(shadowRenderer);

    /*PostProcessing colorCorrection("./assets/shaders/colorCorrection", &engine);
    colorCorrection.onBeforeRenderObservable.add([&]() {
        colorCorrection.shader()->setFloat("gamma", 1.0f / 2.2f);
        colorCorrection.shader()->setFloat("exposure", 1.0f);
        colorCorrection.shader()->setFloat("contrast", 1.0f);
        colorCorrection.shader()->setFloat("saturation", 1.0f);
        colorCorrection.shader()->setFloat("brightness", 0.0f);
    });

    scene.addPostProcess(std::shared_ptr<PostProcessing>(&colorCorrection));*/

    /*PostProcessing invert("./assets/shaders/invertPostProcess", &engine);
    scene.addPostProcess(std::shared_ptr<PostProcessing>(&invert));*/

    PhysicsBody *cloth = new Cloth("cloth", scene, 16, 0.1f);

    cloth->transform()->setRotationZ(-3.14 / 2.0);
    cloth->transform()->setRotationY(3.14);
    cloth->bakeTransformIntoVertexData();
    cloth->transform()->setScale(10);
    cloth->transform()->setPosition(0, 5, 0);

    auto clothMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    clothMaterial->setAlbedoColor(2.0, 2.0, 2.0);
    clothMaterial->setAlbedoTexture(new Texture("./assets/textures/carpet.jpg"));
    clothMaterial->setRoughnessTexture(new Texture("./assets/textures/carpet_roughness.jpg"));
    clothMaterial->setNormalTexture(new Texture("./assets/textures/carpet_normal.png"));
    clothMaterial->setMetallic(0.0f);
    clothMaterial->setAmbientColor(0.02f, 0.02f, 0.02f);
    clothMaterial->setBackFaceCullingEnabled(false);
    clothMaterial->setWireframe(true);
    cloth->mesh()->setMaterial(clothMaterial);

    solver.addBody(cloth);
    solver.applyForce(cloth->mesh(), glm::vec3(0, -9.81, 0));

    shadowRenderer->addShadowCaster(cloth->mesh());

    /*auto sphere = MeshBuilder::makeSphere("sphere", scene, 32);
    sphere->transform()->setPosition(4, 1, 10);

    auto sphereMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    sphereMaterial->setAlbedoTexture(new Texture("./assets/textures/earth.jpg"));
    sphereMaterial->setMetallic(0.8f);
    sphereMaterial->setRoughness(0.4f);
    sphere->setMaterial(sphereMaterial);

    shadowRenderer->addShadowCaster(sphere);*/

    auto ground = MeshBuilder::makePlane("ground", scene, 64);
    ground->transform()->setPosition(0, -2, 0);
    ground->transform()->setScale(40);

    auto groundMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    groundMaterial->setAlbedoColor(0.5, 0.5, 0.5);
    groundMaterial->setMetallic(0.5f);
    groundMaterial->setRoughness(0.4f);
    groundMaterial->receiveShadows(shadowRenderer);

    groundMaterial->setBackFaceCullingEnabled(false);
    ground->setMaterial(groundMaterial);

    bool realTimePhysics = false;

    engine.onKeyPressObservable.add([&](int key) {
        if (key == GLFW_KEY_W) cloth->mesh()->material()->setWireframe(!cloth->mesh()->material()->wireframe());
        if (key == GLFW_KEY_SPACE) realTimePhysics = !realTimePhysics;
        if (!realTimePhysics && key == GLFW_KEY_ENTER) solver.solve(1.0f / 60.0f);
    });

    scene.onBeforeRenderObservable.add([&]() {
        float deltaTime = engine.getDeltaSeconds();

        //glm::vec3 newLightDirection = glm::vec3(cosf(engine.getElapsedTime()), 1.0f, sinf(engine.getElapsedSeconds()));
        //light.setDirection(newLightDirection);

        if (realTimePhysics) solver.solve(1.0 / 60.0f); // fixed time step of 60 fps

        camera.update();
    });

    engine.onExecuteLoopObservable.add([&scene] {
        scene.render();
    });

    engine.start();

    return 0;
}
