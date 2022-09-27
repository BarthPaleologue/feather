//
// Created by barth on 27/09/2022.
//

#ifndef FEATHERGL_FREECAMERA_H
#define FEATHERGL_FREECAMERA_H

#include "Camera.h"

class FreeCamera : public Camera {
public:
    explicit FreeCamera(GLFWwindow *window);

    void update() override;
};


#endif //FEATHERGL_FREECAMERA_H
