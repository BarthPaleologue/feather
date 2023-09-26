//
// Created by barth on 27/09/2022.
//

#ifndef FEATHERGL_FREECAMERA_H
#define FEATHERGL_FREECAMERA_H

#include "Camera.h"

class FreeCamera : public Camera {
public:
    explicit FreeCamera(Engine *engine);

    glm::mat4 computeViewMatrix() override;

    void update() override;
};


#endif //FEATHERGL_FREECAMERA_H
