#include "Scene.h"
#include "cameras/OrbitCamera.h"
#include "Engine.h"
#include "MeshLoader.h"
#include "physics/HpbdSolver.h"
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

    auto clothMesh = MeshBuilder::makePlane("cloth", scene, 8);
    clothMesh->transform()->setRotationZ(-3.14 / 2.0);
    clothMesh->transform()->setRotationY(3.14);
    clothMesh->bakeTransformIntoVertexData();
    clothMesh->transform()->setScale(10);
    clothMesh->bakeTransformIntoVertexData();
    clothMesh->transform()->setPosition(0, 7, 0);

    auto cloth = new SoftBody(clothMesh, 1.0f, 0.02f, 0.01f);
    // Seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the distribution for indices
    std::uniform_int_distribution<> distribution(0, cloth->particles().size() - 1);

    // Generate a random index
    int randomIndex1 = distribution(gen);
    int randomIndex2 = distribution(gen);

    // fixed particles
    auto topLeft = new FixedConstraint(cloth->particles()[randomIndex1], cloth->particles()[randomIndex1]->position);
    cloth->addFixedConstraint(topLeft);

    auto topRight = new FixedConstraint(cloth->particles()[randomIndex2], cloth->particles()[randomIndex2]->position);
    cloth->addFixedConstraint(topRight);

    solver.addBody(cloth);

    /*auto clothWorld = cloth->transform()->computeWorldMatrix();

    auto distanceConstraintMaterial = std::make_shared<BlinnPhongMaterial>(std::shared_ptr<Scene>(&scene));
    distanceConstraintMaterial->setAmbientColor(1.0, 0.0, 0.0);
    for(auto constraint: cloth->distanceConstraints()) {
        if(constraint->particles().size() == 2) {
            auto p1 = constraint->particles()[0];
            auto p2 = constraint->particles()[1];

            auto position1 = clothWorld * glm::vec4(p1->position, 1.0f);
            auto position2 = clothWorld * glm::vec4(p2->position, 1.0f);

            auto line = MeshBuilder::makeLine("line", scene, position1, position2);
            line->setMaterial(distanceConstraintMaterial);
            line->transform()->translate(glm::vec3(1.0, 0.0, 0.0));
        }
    }

    auto bendConstraintMaterial = std::make_shared<BlinnPhongMaterial>(std::shared_ptr<Scene>(&scene));
    bendConstraintMaterial->setAmbientColor(0.0, 1.0, 0.0);
    for(auto constraint: cloth->bendConstraints()) {
        if(constraint->particles().size() == 2) {
            auto p1 = constraint->particles()[0];
            auto p2 = constraint->particles()[1];

            auto position1 = clothWorld * glm::vec4(p1->position, 1.0f);
            auto position2 = clothWorld * glm::vec4(p2->position, 1.0f);

            auto line = MeshBuilder::makeLine("line", scene, position1, position2);
            line->setMaterial(bendConstraintMaterial);
            line->transform()->translate(glm::vec3(2.0, 0.0, 0.0));
        }
    }*/

    auto clothMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    clothMaterial->setAlbedoColor(2.0, 2.0, 2.0);
    clothMaterial->setAlbedoTexture(new Texture("./assets/textures/carpet.jpg"));
    clothMaterial->setRoughnessTexture(new Texture("./assets/textures/carpet_roughness.jpg"));
    clothMaterial->setNormalTexture(new Texture("./assets/textures/carpet_normal.png"));
    clothMaterial->setMetallic(0.0f);
    clothMaterial->setAmbientColor(0.1f, 0.1f, 0.1f);
    clothMaterial->setBackFaceCullingEnabled(false);
    clothMaterial->setWireframe(true);
    cloth->mesh()->setMaterial(clothMaterial);

    shadowRenderer->addShadowCaster(cloth->mesh());

    auto cube = new RigidBody(MeshBuilder::makeUVCube("cube", scene), 1.0);
    cube->transform()->setPosition(5.0, 4, -8.0);

    cube->mesh()->setMaterial(clothMaterial);

    solver.addBody(cube);

    solver.onBeforeSolveObservable.addOnce(
            [&] { cube->particles()[0]->forces.emplace_back(Utils::RandomDirection() * 5000.0f); });

    shadowRenderer->addShadowCaster(cube->mesh());


    /*auto sphere = MeshBuilder::makeSphere("sphere", scene, 32);
    sphere->transform()->setPosition(4, 1, 10);

    auto sphereMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    sphereMaterial->setAlbedoTexture(new Texture("./assets/textures/earth.jpg"));
    sphereMaterial->setMetallic(0.8f);
    sphereMaterial->setRoughness(0.4f);
    sphere->setMaterial(sphereMaterial);

    shadowRenderer->addShadowCaster(sphere);*/

    /*auto bunnyMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    //bunnyMaterial->setWireframe(true);

    float y = 3.0;

    auto bunny = MeshBuilder::FromObjFile("../assets/models/bunny.obj", scene);
    bunny->transform()->setPosition(5, y, -6);
    bunny->setMaterial(bunnyMaterial);
    shadowRenderer->addShadowCaster(bunny);

    auto simplfiedData = bunny->vertexData().vertexSubset().vertexSubset().vertexSubset();
    auto simplifiedBunny1 = Mesh::FromVertexData("simpleBunny", simplfiedData);
    simplifiedBunny1->transform()->setScale(3.0f);
    simplifiedBunny1->bakeTransformIntoVertexData();
    simplifiedBunny1->setMaterial(bunnyMaterial);
    simplifiedBunny1->transform()->translate(glm::vec3(5, y, -2));
    shadowRenderer->addShadowCaster(simplifiedBunny1);

    scene.addMesh(simplifiedBunny1);

    auto softBunny = new SoftBody(simplifiedBunny1, 1.0);
    solver.addBody(softBunny);*/

    /*auto simplifiedBunny2 = MeshBuilder::Simplify("simplifiedBunny2", simplifiedBunny1, scene);
    simplifiedBunny2->setMaterial(bunnyMaterial);
    simplifiedBunny2->transform()->translate(glm::vec3(0, y, 2));
    shadowRenderer->addShadowCaster(simplifiedBunny2);

    auto simplifiedBunny3 = MeshBuilder::Simplify("simplifiedBunny3", simplifiedBunny2, scene);
    simplifiedBunny3->setMaterial(bunnyMaterial);
    simplifiedBunny3->transform()->translate(glm::vec3(0, y, 6));
    shadowRenderer->addShadowCaster(simplifiedBunny3);

    auto soft = new RigidBody(simplifiedBunny3, 1.0);
    solver.addBody(soft);*/

    auto sphere = MeshBuilder::makeSphere("sphere", scene, 8);
    sphere->transform()->translate(glm::vec3(5, 5, 0));
    sphere->setMaterial(clothMaterial);
    shadowRenderer->addShadowCaster(sphere);
    solver.addBody(new SoftBody(sphere, 1.0f));

    auto ground = MeshBuilder::makePlane("ground", scene, 64);
    ground->transform()->setPosition(0, 0, 0);
    ground->transform()->setScale(40);

    auto groundMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    groundMaterial->setAlbedoColor(0.5, 0.5, 0.5);
    groundMaterial->setMetallic(0.5f);
    groundMaterial->setRoughness(0.4f);
    groundMaterial->setBackFaceCullingEnabled(false);
    groundMaterial->receiveShadows(shadowRenderer);

    ground->setMaterial(groundMaterial);

    bool realTimePhysics = false;

    engine.onKeyPressObservable.add([&](int key) {
        if (key == GLFW_KEY_W) cloth->mesh()->material()->setWireframe(!cloth->mesh()->material()->wireframe());
        if (key == GLFW_KEY_N) showNormals(cube->mesh(), scene);
        if (key == GLFW_KEY_R) solver.reset();
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
