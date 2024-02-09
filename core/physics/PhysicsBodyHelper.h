//
// Created by barth on 08/02/24.
//

#ifndef FEATHERGL_PHYSICSBODYHELPER_H
#define FEATHERGL_PHYSICSBODYHELPER_H

#include <memory>
#include "PhysicsBody.h"
#include "ConstraintHelper.h"
#include "BlinnPhongMaterial.h"

class PhysicsBodyHelper {
public:
    PhysicsBodyHelper(const std::shared_ptr<PhysicsBody> &body, std::shared_ptr<Scene> scene) {
        Scene &sceneRef = *scene;
        for (int i = 1; i < body->distanceConstraintsPerLevel().size(); i++) {

            auto material = std::make_shared<BlinnPhongMaterial>(scene);

            if (i == 1) material->setAmbientColor(glm::vec3(1.0, 1.0, 0.0));
            else if (i == 2) material->setAmbientColor(glm::vec3(0.0, 1.0, 1.0));
            else if (i == 3) material->setAmbientColor(glm::vec3(1.0, 0.0, 1.0));

            auto level = body->distanceConstraintsPerLevel()[i];
            for (auto &constraint: level) {
                auto helper = new ConstraintHelper(constraint, sceneRef);
                helper->mesh()->setMaterial(material);
                _constraintHelpers.push_back(helper);
            }
        }

        _aabbHelper = new AABBHelper(body->mesh()->aabb(), sceneRef);
    }

    ~PhysicsBodyHelper() {
        for (auto &constraintHelper: _constraintHelpers) {
            delete constraintHelper;
        }
        delete _aabbHelper;
    }

    void setEnabled(bool enabled) {
        for (auto &constraint: _constraintHelpers) {
            constraint->mesh()->setEnabled(enabled);
        }
        _aabbHelper->mesh()->setEnabled(enabled);
    }

private:
    std::vector<ConstraintHelper *> _constraintHelpers;
    AABBHelper* _aabbHelper;
};

#endif //FEATHERGL_PHYSICSBODYHELPER_H
