#include "Scene.h"
#include "cameras/OrbitCamera.h"
#include "Engine.h"
#include "physics/HpbdSolver.h"
#include "PbrMaterial.h"
#include "physics/RigidBody.h"
#include "physics/SoftBody.h"
#include "physics/UniformAccelerationField.h"
#include "AABBHelper.h"

#include "imgui/imgui.h"

const int WINDOW_WIDTH = 1300;
const int WINDOW_HEIGHT = 800;

float random01() {
    return (float) rand() / (float) RAND_MAX;
}

void showNormals(Mesh *mesh, Scene &scene) {
    for (unsigned int i = 0; i < mesh->vertexData().positions.size(); i += 3) {
        glm::vec3 p1 = glm::vec3(mesh->vertexData().positions[i], mesh->vertexData().positions[i + 1],
                                 mesh->vertexData().positions[i + 2]);
        glm::vec3 n1 = glm::vec3(mesh->vertexData().normals[i], mesh->vertexData().normals[i + 1],
                                 mesh->vertexData().normals[i + 2]);
        glm::vec3 p2 = p1 + n1 * 1.0f;

        auto line = MeshBuilder::makeLine("line", scene, p1, p2);
        line->transform()->setParent(mesh->transform());
    }
}

int main() {
    Engine engine(WINDOW_WIDTH, WINDOW_HEIGHT, "HPBD Cloth Simulation");

    Scene scene((std::shared_ptr<Engine>(&engine)));

    srand(time(0));

    OrbitCamera camera(&engine);
    camera.setRadius(20.0);
    camera.setTarget(glm::vec3(0, 5, 0));
    camera.rotateTheta(-3.14 / 4.0);
    camera.rotatePhi(3.14 / 3.0);
    scene.setActiveCamera(std::shared_ptr<Camera>(&camera));

    DirectionalLight light("sun", -1.0, 1.0, 0.0);
    scene.addDirectionalLight(std::shared_ptr<DirectionalLight>(&light));

    auto shadowRenderer = std::make_shared<ShadowRenderer>(std::shared_ptr<DirectionalLight>(&light), 4096, 4096);
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

    HpbdSolver solver;

    auto gravity = std::make_shared<UniformAccelerationField>(glm::vec3(0.0, -9.81, 0.0));
    solver.addField(gravity);

    const int clothResolution = 32;

    auto clothMesh = MeshBuilder::makePlane("cloth", scene, clothResolution);
    //clothMesh->transform()->setRotationZ(-3.14 / 2.0);
    //clothMesh->transform()->setRotationY(3.14);
    clothMesh->transform()->setScale(10);
    clothMesh->transform()->setPosition(0, 7, 0);

    auto cloth = std::make_shared<SoftBody>(clothMesh, 1.0f, 0.8f, 0.8f);

    // fixed particles
    cloth->addFixedConstraint(new FixedConstraint(cloth->particles()[0]));
    cloth->addFixedConstraint(new FixedConstraint(cloth->particles()[clothResolution - 1]));
    cloth->addFixedConstraint(new FixedConstraint(cloth->particles()[clothResolution * (clothResolution - 1)]));
    cloth->addFixedConstraint(new FixedConstraint(cloth->particles()[clothResolution * clothResolution - 1]));

    solver.addBody(cloth);

    auto clothMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    clothMaterial->setAlbedoColor(2.0, 2.0, 2.0);
    clothMaterial->setAlbedoTexture(new Texture("./assets/textures/carpet.jpg"));
    clothMaterial->setRoughnessTexture(new Texture("./assets/textures/carpet_roughness.jpg"));
    clothMaterial->setNormalTexture(new Texture("./assets/textures/carpet_normal.png"));
    clothMaterial->setMetallic(0.0f);
    clothMaterial->setBackFaceCullingEnabled(false);
    cloth->mesh()->setMaterial(clothMaterial);

    shadowRenderer->addShadowCaster(cloth->mesh());

    auto cubeMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    cubeMaterial->setAlbedoColor(1.0, 0.6, 0.0);
    cubeMaterial->setMetallic(0.2);
    cubeMaterial->setRoughness(0.8);

    auto cube = MeshBuilder::makeUVCube("cube", scene);
    cube->transform()->setPosition(5.0, 4, -8.0);
    cube->setMaterial(cubeMaterial);

    auto cubeBody = std::make_shared<RigidBody>(cube, 1.0f);
    solver.addBody(cubeBody);
    shadowRenderer->addShadowCaster(cube);

    solver.onBeforeSolveObservable.addOnce(
            [&] { cubeBody->particles()[0]->forces.emplace_back(Utils::RandomDirection() * 500.0f); });

    auto cube2 = MeshBuilder::makeUVCube("cube2", scene);
    cube2->transform()->setPosition(-10.0, 4, 0.0);
    cube2->setMaterial(cubeMaterial);
    auto cube2Body = std::make_shared<SoftBody>(cube2, 1.0f, 0.0005f, 0.8f);
    solver.addBody(cube2Body);
    shadowRenderer->addShadowCaster(cube2);

    auto sphere = MeshBuilder::makeIcoSphere("sphere", scene, 2);
    //sphere->transform()->setScale(2.0);
    //sphere->transform()->setPosition(4, 5, 10);
    sphere->transform()->setPosition(0, 9, 0);

    auto sphereMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    sphereMaterial->setAlbedoTexture(new Texture("./assets/textures/earth.jpg"));
    sphereMaterial->setMetallic(0.2f);
    sphereMaterial->setRoughness(0.6f);
    sphere->setMaterial(sphereMaterial);

    shadowRenderer->addShadowCaster(sphere);
    auto sphereBody = std::make_shared<SoftBody>(sphere, 1.0f, 1.0f, 1.0f);
    solver.addBody(sphereBody);

    auto bunnyMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    bunnyMaterial->setAlbedoColor(0.4, 0.4, 1.0);
    bunnyMaterial->setMetallic(0.1);
    bunnyMaterial->setRoughness(0.8);

    auto bunny = MeshBuilder::FromObjFile("../assets/models/bunny.obj", scene);
    bunny->setEnabled(false);

    auto simplifiedBunny = MeshBuilder::Simplify("simpleBunny", bunny.get(), 2, scene);
    simplifiedBunny->setMaterial(bunnyMaterial);
    simplifiedBunny->transform()->translate(glm::vec3(10, 3.0, -8));
    shadowRenderer->addShadowCaster(simplifiedBunny);

    auto softBunny = std::make_shared<SoftBody>(simplifiedBunny, 1.0, 1.0f, 1.0f);
    softBunny->generalizedVolumeConstraints()[0]->setPressure(1.2f);

    solver.addBody(softBunny);

    auto rawDress = MeshBuilder::FromObjFile("../assets/models/dress/untitled.obj", scene);
    rawDress->setEnabled(false);

    auto dress = MeshBuilder::Simplify("dress", rawDress.get(), 1, scene);
    dress->vertexData().computeNormals();
    dress->transform()->setScale(0.4f);
    dress->transform()->setPosition(10, 5, 4);
    shadowRenderer->addShadowCaster(dress);

    auto dressMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    dressMaterial->setAlbedoColor(1.0, 0.0, 0.0);
    dressMaterial->setMetallic(0.1);
    dressMaterial->setRoughness(0.4);
    dressMaterial->setBackFaceCullingEnabled(false);
    dress->setMaterial(dressMaterial);

    auto dressBody = std::make_shared<SoftBody>(dress, 1.0, 0.5f, 0.5f);
    solver.addBody(dressBody);

    auto ground = MeshBuilder::makePlane("ground", scene, 2);
    ground->transform()->setPosition(0, 0, 0);
    ground->transform()->setScale(40);

    auto groundMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    groundMaterial->setAlbedoColor(0.5, 0.5, 0.5);
    groundMaterial->setMetallic(0.5f);
    groundMaterial->setRoughness(0.4f);
    groundMaterial->setBackFaceCullingEnabled(false);
    groundMaterial->receiveShadows(shadowRenderer);

    ground->setMaterial(groundMaterial);

    auto groundBody = std::make_shared<RigidBody>(ground, 0.0f);
    solver.addBody(groundBody);

    // display bounding boxes
    /*for(const auto& mesh: scene.meshes()) {
        new AABBHelper(mesh->aabb(), scene);
    }*/


    // Seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the distribution for indices
    std::uniform_real_distribution<> distribution(-10.0, 10.0);

    bool realTimePhysics = false;
    float bunnyPressure = 1.0f;
    bool wireframe = false;

    scene.onRenderGuiObservable.add([&] {
        ImGui::Begin("HPBD Controls");

        // set bunny pressure
        ImGui::SliderFloat("Bunny pressure", &bunnyPressure, 0.0f, 4.0f);
        softBunny->generalizedVolumeConstraints()[0]->setPressure(bunnyPressure);

        // set wireframe
        ImGui::Checkbox("Wireframe", &wireframe);
        clothMaterial->setWireframe(wireframe);
        sphereMaterial->setWireframe(wireframe);
        cubeMaterial->setWireframe(wireframe);
        bunnyMaterial->setWireframe(wireframe);
        groundMaterial->setWireframe(wireframe);
        dressMaterial->setWireframe(wireframe);

        // start button
        if (ImGui::Button("Toggle simulation")) {
            realTimePhysics = !realTimePhysics;
        }
        ImGui::SameLine();


        // physics step button
        if (ImGui::Button("Step simulation")) {
            solver.solve(1.0f / 60.0f);
        }
        ImGui::SameLine();

        // reset button
        if (ImGui::Button("Reset simulation")) {
            solver.reset();
        }

        ImGui::End();
    });


    engine.onKeyPressObservable.add([&](int key) {
        if (key == GLFW_KEY_W) {
            wireframe = !wireframe;
        }
        if (key == GLFW_KEY_R) solver.reset();
        if (key == GLFW_KEY_SPACE) realTimePhysics = !realTimePhysics;
        if (!realTimePhysics && key == GLFW_KEY_ENTER) solver.solve(1.0f / 60.0f);
    });

    int i = 0;
    scene.onBeforeRenderObservable.add([&]() {
        float deltaTime = engine.getDeltaSeconds();

        /*if(realTimePhysics && i % 150 == 0) {
            auto cube = MeshBuilder::makeUVCube("cube", scene);
            cube->transform()->setPosition((float)distribution(gen), 10, (float)distribution(gen));
            cube->setMaterial(cubeMaterial);

            auto cubeBody = std::make_shared<RigidBody>(cube, 1.0f);
            solver.addBody(cubeBody);
            shadowRenderer->addShadowCaster(cube);

            cubeBody->particles()[0]->forces.emplace_back(Utils::RandomDirection() * 20.0f);
        }
        if(realTimePhysics) i++;*/

        float theta = 0.1f * engine.getElapsedSeconds() + 3.14f;
        glm::vec3 newLightDirection = glm::normalize(glm::vec3(cosf(theta), 1.0f, sinf(theta)));
        light.setDirection(newLightDirection);

        if (realTimePhysics) solver.solve(1.0 / 60.0f); // fixed time step of 60 fps

        camera.update();
    });

    engine.onExecuteLoopObservable.add([&scene] {
        scene.render();
    });

    engine.start();

    return 0;
}
