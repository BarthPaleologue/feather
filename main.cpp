#include "Scene.h"
#include "cameras/OrbitCamera.h"
#include "Engine.h"
#include "physics/HXPbdSolver.h"
#include "PbrMaterial.h"
#include "physics/RigidBody.h"
#include "physics/SoftBody.h"
#include "physics/UniformAccelerationField.h"
#include "AABBHelper.h"

#include <glm/gtx/norm.hpp>
#include "imgui/imgui.h"
#include "physics/PhysicsBodyHelper.h"

/*void showNormals(Mesh *mesh, Scene &scene) {
    for (unsigned int i = 0; i < mesh->vertexData().positions.size(); i += 3) {
        glm::vec3 p1 = glm::vec3(mesh->vertexData().positions[i], mesh->vertexData().positions[i + 1],
                                 mesh->vertexData().positions[i + 2]);
        glm::vec3 n1 = glm::vec3(mesh->vertexData().normals[i], mesh->vertexData().normals[i + 1],
                                 mesh->vertexData().normals[i + 2]);
        glm::vec3 p2 = p1 + n1 * 1.0f;

        auto line = MeshBuilder::makeLine("line", scene, p1, p2);
        line->transform()->setParent(mesh->transform());
    }
}*/

int main() {
    Engine engine(1300, 800, "HPBD Cloth Simulation");

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

    PostProcessing colorCorrection("./assets/shaders/colorCorrection", &engine);
    colorCorrection.onBeforeRenderObservable.add([&]() {
        colorCorrection.shader()->setFloat("gamma", 1.0f);
        colorCorrection.shader()->setFloat("exposure", 1.0f);
        colorCorrection.shader()->setFloat("contrast", 1.1f);
        colorCorrection.shader()->setFloat("saturation", 1.1f);
        colorCorrection.shader()->setFloat("brightness", 0.0f);
    });

    scene.addPostProcess(std::shared_ptr<PostProcessing>(&colorCorrection));

    HXPbdSolver solver;

    auto gravity = std::make_shared<UniformAccelerationField>(glm::vec3(0.0, -9.81, 0.0));
    solver.addField(gravity);

    std::map<std::shared_ptr<PhysicsBody>, PhysicsBodyHelper *> bodyHelpers;

    const int clothResolution = 32;

    auto clothMesh = MeshBuilder::makePlane("cloth", scene, clothResolution);
    clothMesh->transform()->setScale(10);
    clothMesh->transform()->setPosition(0, 7, 0);

    auto cloth = std::make_shared<SoftBody>(clothMesh, 1.0f, 0.1f, 0.01f);

    // fixed particles
    cloth->addFixedConstraint(std::make_shared<FixedConstraint>(cloth->particles()[0]));
    cloth->addFixedConstraint(std::make_shared<FixedConstraint>(cloth->particles()[clothResolution - 1]));
    cloth->addFixedConstraint(std::make_shared<FixedConstraint>(cloth->particles()[clothResolution * (clothResolution - 1)]));
    cloth->addFixedConstraint(std::make_shared<FixedConstraint>(cloth->particles()[clothResolution * clothResolution - 1]));

    solver.addBody(cloth);

    auto clothMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    clothMaterial->setAlbedoColor(2.0, 2.0, 2.0);
    clothMaterial->setAlbedoTexture(new Texture("./assets/textures/carpet.jpg"));
    clothMaterial->setRoughnessTexture(new Texture("./assets/textures/carpet_roughness.jpg"));
    clothMaterial->setNormalTexture(new Texture("./assets/textures/carpet_normal.png"));
    clothMaterial->setMetallic(0.0f);
    clothMaterial->setBackFaceCullingEnabled(false);
    clothMaterial->receiveShadows(shadowRenderer);
    cloth->mesh()->setMaterial(clothMaterial);

    shadowRenderer->addShadowCaster(cloth->mesh());

    bodyHelpers.insert({cloth, new PhysicsBodyHelper(cloth, std::shared_ptr<Scene>(&scene))});

    auto cloth2Mesh = MeshBuilder::makePlane("cloth2", scene, clothResolution);
    cloth2Mesh->transform()->setScale(10);
    cloth2Mesh->transform()->setPosition(0, 7, 0);
    cloth2Mesh->setMaterial(clothMaterial);
    shadowRenderer->addShadowCaster(cloth2Mesh);
    auto cloth2 = std::make_shared<SoftBody>(cloth2Mesh, 1.0f, 0.1f, 0.01f);
    solver.addBody(cloth2);

    bodyHelpers.insert({cloth2, new PhysicsBodyHelper(cloth2, std::shared_ptr<Scene>(&scene))});

    // fixed particles
    cloth2->addFixedConstraint(std::make_shared<FixedConstraint>(cloth2->particles()[0]));
    cloth2->addFixedConstraint(
            std::make_shared<FixedConstraint>(cloth2->particles()[clothResolution * clothResolution / 2 + clothResolution / 2]));

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

    bodyHelpers.insert({cubeBody, new PhysicsBodyHelper(cubeBody, std::shared_ptr<Scene>(&scene))});

    solver.onBeforeSolveObservable.addOnce(
            [&] { cubeBody->particles()[0]->externalForces.emplace_back(Utils::RandomDirection() * 500.0f); });

    auto sphere = MeshBuilder::makeIcoSphere("sphere", scene, 2);
    sphere->transform()->setScale(1.5);
    //sphere->transform()->setPosition(4, 5, 10);
    sphere->transform()->setPosition(0, 9, 0);

    auto sphereMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    sphereMaterial->setAlbedoTexture(new Texture("./assets/textures/earth.jpg"));
    sphereMaterial->setMetallic(0.2f);
    sphereMaterial->setRoughness(0.6f);
    sphere->setMaterial(sphereMaterial);

    shadowRenderer->addShadowCaster(sphere);
    auto sphereBody = std::make_shared<SoftBody>(sphere, 1.0f, 0.0001f, 0.0001f);
    solver.addBody(sphereBody);

    bodyHelpers.insert({sphereBody, new PhysicsBodyHelper(sphereBody, std::shared_ptr<Scene>(&scene))});

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

    auto softBunny = std::make_shared<SoftBody>(simplifiedBunny, 1.0, 0.0001f, 0.0001f);
    solver.addBody(softBunny);

    //bodyHelpers.insert({softBunny, new PhysicsBodyHelper(softBunny, std::shared_ptr<Scene>(&scene))});

    auto armadillo = MeshBuilder::FromObjFile("../assets/models/armadillo.obj", scene);
    auto simplifiedData = armadillo->vertexData();
    for (unsigned int i = 0; i < 3; i++) {
        simplifiedData = simplifiedData.simplify();
    }
    armadillo->setVertexData(simplifiedData);
    armadillo->setMaterial(cubeMaterial);
    armadillo->transform()->setPosition(5, 5, 10);
    armadillo->transform()->setScale(2.0f);
    shadowRenderer->addShadowCaster(armadillo);

    auto armadilloBody = std::make_shared<RigidBody>(armadillo, 1.0f);
    armadilloBody->addGlobalVolumeConstraint(
            std::make_shared<GlobalVolumeConstraint>(armadilloBody->particles(), armadillo->vertexData().indices, 0.0001f,
                                       0.0001f));
    solver.addBody(armadilloBody);

    bodyHelpers.insert({armadilloBody, new PhysicsBodyHelper(armadilloBody, std::shared_ptr<Scene>(&scene))});

    auto ground = MeshBuilder::makePlane("ground", scene, 2);
    ground->transform()->setPosition(0, 0, 0);
    ground->transform()->setScale(40);
    ground->setPickingEnabled(false);

    auto groundMaterial = std::make_shared<PbrMaterial>(std::shared_ptr<Scene>(&scene));
    groundMaterial->setAlbedoColor(0.5, 0.5, 0.5);
    groundMaterial->setMetallic(0.5f);
    groundMaterial->setRoughness(0.4f);
    groundMaterial->setBackFaceCullingEnabled(false);
    groundMaterial->receiveShadows(shadowRenderer);

    ground->setMaterial(groundMaterial);

    auto groundBody = std::make_shared<RigidBody>(ground, 0.0f);
    solver.addBody(groundBody);

    // for all bodyHelpers, set enabled to false
    for (const auto &bodyHelper: bodyHelpers) {
        bodyHelper.second->setEnabled(false);
    }

    // Seed the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());

    // Define the distribution for indices
    std::uniform_real_distribution<> distribution(-10.0, 10.0);

    bool realTimePhysics = false;
    bool fixedTimeStep = true;
    bool wireframe = false;
    bool bodyHelperEnabled = false;

    bool clothEnabled = true;
    bool cloth2Enabled = false;
    bool cubeEnabled = false;
    bool sphereEnabled = true;
    bool bunnyEnabled = true;
    bool armadilloEnabled = false;
    int nbIterations = 4;

    std::shared_ptr<PhysicsBody> currentBody = softBunny;
    float currentBodyPressure = 1.0f;
    float stretchCompliance = 0.0001f;
    float bendCompliance = 0.0001f;
    float fixedConstraintCompliance = 0.0f;

    std::vector<std::shared_ptr<PhysicsBody>> additionalBodies;

    scene.onRenderGuiObservable.add([&] {
        ImGui::Begin("HPBD Controls");

        // show number of FPS
        ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);

        // show number of particles
        ImGui::Text("Nb particles: %d", solver.nbParticles());

        // set solver nb iterations
        ImGui::SliderInt("Solver iterations", &nbIterations, 1, 20);
        solver.setIterations(nbIterations);

        if (currentBody != nullptr) {
            ImGui::Text("Selected body: %s", currentBody->mesh()->name().c_str());
        }

        if (currentBody != nullptr && bodyHelpers.find(currentBody) != bodyHelpers.end()) {
            ImGui::Checkbox("Show helper", &bodyHelperEnabled);
            bodyHelpers[currentBody]->setEnabled(bodyHelperEnabled);
        }

        if (currentBody != nullptr && !currentBody->globalVolumeConstraints().empty()) {
            // pressure slider
            ImGui::SliderFloat("Pressure", &currentBodyPressure, 0.0001f, 4.0f);
            currentBody->globalVolumeConstraints()[0]->setPressure(currentBodyPressure);
        }

        if (currentBody != nullptr && !currentBody->distanceConstraints().empty()) {
            // stretch compliance slider
            ImGui::SliderFloat("Stretch compliance", &stretchCompliance, 0.0001f, 2.0f, "%.4f");
            for (const auto &constraint: currentBody->distanceConstraints()) {
                if (constraint->compliance() > 0.0f) constraint->setCompliance(stretchCompliance);
            }
            // apply compliance to hierarchy
            for (const auto &level: currentBody->distanceConstraintsPerLevel()) {
                for (const auto &constraint: level) {
                    if (constraint->compliance() > 0.0f) constraint->setCompliance(stretchCompliance);
                }
            }
        }

        if (currentBody != nullptr && !currentBody->fastBendConstraints().empty()) {
            // bend compliance slider
            ImGui::SliderFloat("Bend compliance", &bendCompliance, 0.0001f, 2.0f, "%.4f");
            for (const auto &constraint: currentBody->fastBendConstraints()) {
                constraint->setCompliance(bendCompliance);
            }
        }

        if (currentBody != nullptr && !currentBody->fixedConstraints().empty()) {
            // fixed constraint compliance slider
            ImGui::SliderFloat("Fixed constraint compliance", &fixedConstraintCompliance, 0.0f, 30.0f, "%.4f");
            for (const auto &constraint: currentBody->fixedConstraints()) {
                constraint->setCompliance(fixedConstraintCompliance);
            }
        }

        // fixed time step
        ImGui::Checkbox("Fixed time step", &fixedTimeStep);

        // set wireframe
        ImGui::Checkbox("Wireframe", &wireframe);
        clothMaterial->setWireframe(wireframe);
        sphereMaterial->setWireframe(wireframe);
        cubeMaterial->setWireframe(wireframe);
        bunnyMaterial->setWireframe(wireframe);
        groundMaterial->setWireframe(wireframe);

        // set enabled
        ImGui::Checkbox("Cloth enabled", &clothEnabled);
        cloth->mesh()->setEnabled(clothEnabled);

        ImGui::Checkbox("Cloth2 enabled", &cloth2Enabled);
        cloth2->mesh()->setEnabled(cloth2Enabled);

        ImGui::Checkbox("Cube enabled", &cubeEnabled);
        cube->setEnabled(cubeEnabled);

        ImGui::Checkbox("Sphere enabled", &sphereEnabled);
        sphere->setEnabled(sphereEnabled);

        ImGui::Checkbox("Bunny enabled", &bunnyEnabled);
        simplifiedBunny->setEnabled(bunnyEnabled);

        ImGui::Checkbox("Armadillo enabled", &armadilloEnabled);
        armadillo->setEnabled(armadilloEnabled);

        // spawn sphere button
        if (ImGui::Button("Spawn sphere")) {
            auto sphere = MeshBuilder::makeIcoSphere("sphere", scene, 2);
            sphere->transform()->setScale(1.5);
            sphere->transform()->setPosition(distribution(gen), 10, distribution(gen));
            sphere->setMaterial(sphereMaterial);
            shadowRenderer->addShadowCaster(sphere);
            auto sphereBody = std::make_shared<SoftBody>(sphere, 1.0f, 0.01f, 0.01f);
            solver.addBody(sphereBody);
            additionalBodies.push_back(sphereBody);
        }
        ImGui::SameLine();

        // spawn cube button
        if (ImGui::Button("Spawn cube")) {
            auto cube = MeshBuilder::makeUVCube("cube", scene);
            cube->transform()->setPosition(distribution(gen), 11, distribution(gen));
            cube->setMaterial(cubeMaterial);
            auto cubeBody = std::make_shared<RigidBody>(cube, 1.0f);
            solver.addBody(cubeBody);
            shadowRenderer->addShadowCaster(cube);
            additionalBodies.push_back(cubeBody);

            solver.onBeforeSolveObservable.addOnce(
                    [cubeBody] {
                        cubeBody->particles()[0]->externalForces.emplace_back(Utils::RandomDirection() * 50.0f);
                    });
        }
        ImGui::SameLine();

        // spawn bunny button
        if (ImGui::Button("Spawn bunny")) {
            auto bunny = MeshBuilder::FromObjFile("../assets/models/bunny.obj", scene);
            auto simplifiedData = bunny->vertexData();
            for (unsigned int i = 0; i < 2; i++) {
                simplifiedData = simplifiedData.simplify();
            }
            bunny->setVertexData(simplifiedData);
            bunny->setMaterial(bunnyMaterial);
            bunny->transform()->setPosition(distribution(gen), 12, distribution(gen));
            shadowRenderer->addShadowCaster(bunny);
            auto bunnyBody = std::make_shared<SoftBody>(bunny, 1.0, 0.01f, 0.01f);
            solver.addBody(bunnyBody);

            additionalBodies.push_back(bunnyBody);
        }

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
            for(const auto& body: additionalBodies) {
                solver.removeBody(body);
                scene.removeMesh(body->mesh());
                shadowRenderer->removeShadowCaster(body->mesh());
            }
            additionalBodies.clear();
        }

        ImGui::End();
    });


    engine.onKeyPressObservable.add([&](int key) {
        if (key == GLFW_KEY_W) {
            wireframe = !wireframe;
        }
        if (key == GLFW_KEY_SPACE) realTimePhysics = !realTimePhysics;
        if (!realTimePhysics && key == GLFW_KEY_ENTER) solver.solve(1.0f / 60.0f);
    });

    bool isDraggingABody = false;
    std::shared_ptr<PhysicsBody> draggedBody = nullptr;
    int draggedFace = 0;
    float dragDistance = 0.0;

    scene.onBeforeRenderObservable.add([&]() {
        float deltaTime = engine.getDeltaSeconds();

        if (!engine.isMousePressed()) isDraggingABody = false;
        if (engine.isMousePressed() && engine.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
            glm::vec3 rayOrigin = camera.position();
            glm::vec2 mousePos = engine.getMousePosition();
            glm::vec2 windowSize = engine.getWindowSize();

            glm::vec3 rayDirection = camera.getRayFromMouse(mousePos.x, mousePos.y, windowSize.x, windowSize.y);

            if (!isDraggingABody) {
                auto pickResult = scene.pickWithRay(rayOrigin, rayDirection);

                if (pickResult.second.hasHit) {
                    auto pickedMesh = pickResult.first;

                    // find associated physics body
                    for (const auto &body: solver.physicsBodies()) {
                        if (body->mesh() == pickedMesh) {
                            currentBody = body;
                            if (!currentBody->globalVolumeConstraints().empty()) currentBodyPressure = currentBody->globalVolumeConstraints()[0]->pressure();
                            if (!currentBody->distanceConstraints().empty()) stretchCompliance = currentBody->distanceConstraints()[0]->compliance();
                            if (!currentBody->fastBendConstraints().empty()) bendCompliance = currentBody->fastBendConstraints()[0]->compliance();
                            if (!currentBody->fixedConstraints().empty()) fixedConstraintCompliance = currentBody->fixedConstraints()[0]->compliance();
                            break;
                        }
                    }

                    auto hitPoint = pickResult.second.hitPoint;
                    isDraggingABody = true;
                    draggedBody = currentBody;
                    dragDistance = glm::length(hitPoint - rayOrigin);
                    draggedFace = pickResult.second.faceIndex;
                }
            }

            if (isDraggingABody) {
                int index0 = currentBody->mesh()->vertexData().indices[draggedFace * 3];
                int index1 = currentBody->mesh()->vertexData().indices[draggedFace * 3 + 1];
                int index2 = currentBody->mesh()->vertexData().indices[draggedFace * 3 + 2];

                auto particle0 = currentBody->particles()[index0];
                auto particle1 = currentBody->particles()[index1];
                auto particle2 = currentBody->particles()[index2];

                glm::vec3 t0 = particle0->position;
                glm::vec3 t1 = particle1->position;
                glm::vec3 t2 = particle2->position;

                glm::vec3 barycenter = (t0 + t1 + t2) / 3.0f;

                glm::vec3 hitPoint = rayOrigin + rayDirection * dragDistance;

                glm::vec3 translation = hitPoint - barycenter;

                for (const auto &particle: currentBody->particles()) {
                    float weight = 1.0f / (1.0f + glm::length(particle->position - barycenter));
                    particle->position += translation * weight;
                }

                currentBody->mesh()->vertexData().computeNormals();

                currentBody->mesh()->sendVertexDataToGPU();
            }

        }

        float theta = 0.1f * engine.getElapsedSeconds() + 3.14f;
        glm::vec3 newLightDirection = glm::normalize(glm::vec3(cosf(theta), 1.0f, sinf(theta)));
        light.setDirection(newLightDirection);

        if (realTimePhysics) solver.solve(fixedTimeStep ? 1.0f / 60.0f : deltaTime); // fixed time step of 60 fps

        camera.update();
    });

    engine.onExecuteLoopObservable.add([&scene] {
        scene.render();
    });

    engine.start();

    return 0;
}
