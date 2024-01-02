#include "Scene.h"
#include "cameras/OrbitCamera.h"
#include "lights/PointLight.h"
#include "CelestialBody.h"
#include "Engine.h"
#include "MeshLoader.h"
#include "physics/HpbdSolver.h"
#include "physics/Cloth.h"
#include "PbrMaterial.h"
#include "physics/RigidBody.h"
#include "physics/SoftBody.h"
#include "physics/UniformAccelerationField.h"

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 600;

float random01() {
    return (float) rand() / (float) RAND_MAX;
}

void showNormals(std::shared_ptr<Mesh> mesh, Scene &scene) {
    for (unsigned int i = 0; i < mesh->vertexData().positions.size(); i += 3) {
        glm::vec3 p1 = glm::vec3(mesh->vertexData().positions[i], mesh->vertexData().positions[i + 1],
                                 mesh->vertexData().positions[i + 2]);
        glm::vec3 n1 = glm::vec3(mesh->vertexData().normals[i], mesh->vertexData().normals[i + 1],
                                 mesh->vertexData().normals[i + 2]);
        std::cout << glm::length(n1) << std::endl;
        glm::vec3 p2 = p1 + n1 * 1.0f;

        auto line = MeshBuilder::makeLine("line", scene, p1, p2);
        line->transform()->setParent(mesh->transform());
    }
}

int main() {
    Engine engine(WINDOW_WIDTH, WINDOW_HEIGHT, "HPBD Cloth Simulation");

    Scene scene((std::shared_ptr<Engine>(&engine)));

    OrbitCamera camera(&engine);
    camera.setRadius(20.0);
    camera.setTarget(glm::vec3(0, 1, 0));
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

    HpbdSolver solver;

    auto gravity = std::make_shared<UniformAccelerationField>(glm::vec3(0.0, -9.81, 0.0));
    solver.addField(gravity);

    /*auto cloth = new Cloth("cloth", scene, 32, 10.0f);

    cloth->transform()->setRotationZ(-3.14 / 2.0);
    cloth->transform()->setRotationY(3.14);
    cloth->bakeTransformIntoVertexData();
    cloth->transform()->setScale(10);
    cloth->transform()->setPosition(0, 7, 0);
    cloth->bakeTransformIntoVertexData();

    auto simplifiedVertexData = cloth->mesh()->vertexData().vertexSubset();
    auto simplifiedMesh = Mesh::FromVertexData("simplified1", simplifiedVertexData);
    simplifiedMesh->material()->setWireframe(true);
    simplifiedMesh->transform()->translate(glm::vec3(0, 0, -15));
    scene.addMesh(simplifiedMesh);

    auto simplifiedVertexData2 = simplifiedMesh->vertexData().vertexSubset();
    auto simplifiedMesh2 = Mesh::FromVertexData("simplified2", simplifiedVertexData2);
    simplifiedMesh2->material()->setWireframe(true);
    simplifiedMesh2->transform()->translate(glm::vec3(0, 0, -30));
    scene.addMesh(simplifiedMesh2);

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

    shadowRenderer->addShadowCaster(cloth->mesh());

    auto cube = new RigidBody(MeshBuilder::makeUVCube("cube", scene), 1.0);
    cube->transform()->setPosition(5.0, 4, -8.0);

    cube->mesh()->setMaterial(clothMaterial);

    solver.addBody(cube);

    solver.onBeforeSolveObservable.addOnce([&] { cube->particles()[0]->forces.emplace_back(0, 5000.0f, 0); });

    shadowRenderer->addShadowCaster(cube->mesh());*/


    /*auto sphere = MeshBuilder::makeSphere("sphere", scene, 32);
    sphere->transform()->setPosition(4, 1, 10);

    auto sphereMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    sphereMaterial->setAlbedoTexture(new Texture("./assets/textures/earth.jpg"));
    sphereMaterial->setMetallic(0.8f);
    sphereMaterial->setRoughness(0.4f);
    sphere->setMaterial(sphereMaterial);

    shadowRenderer->addShadowCaster(sphere);*/

    /*auto bunny = MeshBuilder::FromObjFile("../assets/models/bunny.obj", scene);
    bunny->transform()->setScale(5);
    bunny->bakeTransformIntoVertexData();
    bunny->transform()->setPosition(-10, 5, 0);*/

    auto bunnyMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    bunnyMaterial->setWireframe(true);

    float y = 3.0;

    auto bunny = MeshBuilder::FromObjFile("../assets/models/bunny.obj", scene);
    //bunny->transform()->setScale(2);
    //bunny->bakeTransformIntoVertexData();
    bunny->transform()->setPosition(0, y, -6);
    bunny->setMaterial(bunnyMaterial);
    shadowRenderer->addShadowCaster(bunny);

    auto simplifiedBunnyData1 = bunny->vertexData().vertexSubset();
    auto simplifiedBunny1 = Mesh::FromVertexData("simplifiedBunny1", simplifiedBunnyData1);
    simplifiedBunny1->setMaterial(bunnyMaterial);
    simplifiedBunny1->transform()->translate(glm::vec3(0, y, -2));
    scene.addMesh(simplifiedBunny1);
    shadowRenderer->addShadowCaster(simplifiedBunny1);

    auto simplifiedBunnyData2 = simplifiedBunny1->vertexData().vertexSubset();
    auto simplifiedBunny2 = Mesh::FromVertexData("simplifiedBunny2", simplifiedBunnyData2);
    simplifiedBunny2->setMaterial(bunnyMaterial);
    simplifiedBunny2->transform()->translate(glm::vec3(0, y, 2));
    scene.addMesh(simplifiedBunny2);
    shadowRenderer->addShadowCaster(simplifiedBunny2);

    auto simplifiedBunnyData3 = simplifiedBunny2->vertexData().vertexSubset();
    auto simplifiedBunny3 = Mesh::FromVertexData("simplifiedBunny3", simplifiedBunnyData3);
    simplifiedBunny3->setMaterial(bunnyMaterial);
    simplifiedBunny3->transform()->translate(glm::vec3(0, y, 6));
    scene.addMesh(simplifiedBunny3);
    shadowRenderer->addShadowCaster(simplifiedBunny3);


    auto soft = new RigidBody(simplifiedBunny3, 1.0);
    solver.addBody(soft);

    auto ground = MeshBuilder::makePlane("ground", scene, 64);
    ground->transform()->setPosition(0, 0, 0);
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
        //if (key == GLFW_KEY_W) cloth->mesh()->material()->setWireframe(!cloth->mesh()->material()->wireframe());
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
