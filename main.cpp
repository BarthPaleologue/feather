#include "Scene.h"
#include "cameras/OrbitCamera.h"
#include "Engine.h"
#include "MeshLoader.h"
#include "physics/HpbdSolver.h"
#include "PbrMaterial.h"
#include "physics/RigidBody.h"
#include "physics/SoftBody.h"
#include "physics/UniformAccelerationField.h"

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

    auto clothMesh = MeshBuilder::makePlane("cloth", scene, 16);
    clothMesh->transform()->setRotationZ(-3.14 / 2.0);
    clothMesh->transform()->setRotationY(3.14);
    clothMesh->transform()->setScale(10);
    clothMesh->transform()->setPosition(0, 7, 0);

    auto cloth = new SoftBody(clothMesh, 1.0f, 0.02f, 0.02f);
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
    cloth->mesh()->setMaterial(clothMaterial);

    shadowRenderer->addShadowCaster(cloth->mesh());

    auto cubeMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    cubeMaterial->setAlbedoColor(1.0, 0.0, 0.0);
    cubeMaterial->setMetallic(1.0f);
    cubeMaterial->setRoughness(0.4f);

    auto cube = MeshBuilder::makeUVCube("cube", scene);
    cube->transform()->setPosition(5.0, 4, -8.0);
    cube->setMaterial(cubeMaterial);

    auto cubeBody = new RigidBody(cube, 1.0f);
    solver.addBody(cubeBody);
    shadowRenderer->addShadowCaster(cube);

    solver.onBeforeSolveObservable.addOnce(
            [&] { cubeBody->particles()[0]->forces.emplace_back(Utils::RandomDirection() * 500.0f); });

    auto cube2 = MeshBuilder::makeUVCube("cube2", scene);
    cube2->transform()->setPosition(-7.0, 4, 0.0);
    cube2->setMaterial(cubeMaterial);
    auto cube2Body = new SoftBody(cube2, 1.0f, 0.0005f, 0.0005f);
    solver.addBody(cube2Body);
    shadowRenderer->addShadowCaster(cube2);

    auto sphere = MeshBuilder::makeIcoSphere("sphere", scene, 2);
    sphere->transform()->setScale(2.0);
    sphere->transform()->setPosition(4, 5, 10);

    auto sphereMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    sphereMaterial->setAlbedoTexture(new Texture("./assets/textures/earth.jpg"));
    sphereMaterial->setMetallic(0.2f);
    sphereMaterial->setRoughness(0.6f);
    sphere->setMaterial(sphereMaterial);

    shadowRenderer->addShadowCaster(sphere);
    auto sphereBody = new SoftBody(sphere, 1.0f, 0.2f, 0.2f);
    solver.addBody(sphereBody);

    solver.onBeforeSolveObservable.addOnce(
            [&] { sphereBody->particles()[0]->forces.emplace_back(Utils::RandomDirection() * 50.0f); });

    auto bunnyMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    bunnyMaterial->setAlbedoColor(0.4, 0.4, 1.0);
    bunnyMaterial->setMetallic(0.1);
    bunnyMaterial->setRoughness(0.4);

    auto bunny = MeshBuilder::FromObjFile("../assets/models/bunny.obj", scene);
    bunny->setEnabled(false);

    auto simplifiedBunny = MeshBuilder::Simplify("simpleBunny", bunny.get(), 2, scene);
    simplifiedBunny->setMaterial(bunnyMaterial);
    simplifiedBunny->transform()->translate(glm::vec3(10, 3.0, -2));
    shadowRenderer->addShadowCaster(simplifiedBunny);

    auto softBunny = new SoftBody(simplifiedBunny, 1.0, 0.5f, 0.5f);
    solver.addBody(softBunny);

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

    auto groundBody = new RigidBody(ground, 0.0f);
    solver.addBody(groundBody);

    auto createCollisionsConstraints = [](PhysicsBody *collider, PhysicsBody *collided) {
        auto groundParticles = collided->particles();
        for(auto particle: collider->particles()) {
            for(unsigned int i = 0; i < collided->mesh()->vertexData().indices.size(); i+=3) {
                auto index1 = collided->mesh()->vertexData().indices[i];
                auto index2 = collided->mesh()->vertexData().indices[i + 1];
                auto index3 = collided->mesh()->vertexData().indices[i + 2];
                auto p1 = groundParticles[index1];
                auto p2 = groundParticles[index2];
                auto p3 = groundParticles[index3];
                auto constraint = new CollisionConstraint(particle, p1, p2, p3, 0.01f);
                collider->addCollisionConstraint(constraint);
            }
        }
    };

    createCollisionsConstraints(cubeBody, groundBody);
    createCollisionsConstraints(softBunny, groundBody);
    createCollisionsConstraints(cloth, groundBody);
    createCollisionsConstraints(cube2Body, groundBody);
    createCollisionsConstraints(sphereBody, groundBody);
    //createCollisionsConstraints(softBunny, cubeBody);

    bool realTimePhysics = false;

    engine.onKeyPressObservable.add([&](int key) {
        if (key == GLFW_KEY_W) {
            clothMaterial->setWireframe(!clothMaterial->wireframe());
            sphereMaterial->setWireframe(!sphereMaterial->wireframe());
            cubeMaterial->setWireframe(!cubeMaterial->wireframe());
            bunnyMaterial->setWireframe(!bunnyMaterial->wireframe());
            groundMaterial->setWireframe(!groundMaterial->wireframe());
        }
        if (key == GLFW_KEY_R) solver.reset();
        if (key == GLFW_KEY_SPACE) realTimePhysics = !realTimePhysics;
        if (!realTimePhysics && key == GLFW_KEY_ENTER) solver.solve(1.0f / 60.0f);
    });

    scene.onBeforeRenderObservable.add([&]() {
        float deltaTime = engine.getDeltaSeconds();

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
