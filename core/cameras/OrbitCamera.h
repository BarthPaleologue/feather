//
// Created by barth on 27/09/2022.
//

#ifndef FEATHERGL_ORBITCAMERA_H
#define FEATHERGL_ORBITCAMERA_H

#include "Camera.h"

class OrbitCamera: public Camera {
public:
    explicit OrbitCamera(GLFWwindow *window);

    glm::mat4 computeViewMatrix() override;

    void setTarget(glm::vec3 *target);

    void setRadius(float radius);

    void zoom(float amount);

    void rotateTheta(float angle);

    void rotatePhi(float angle);

    void update() override;

private:
    glm::vec3 _target;
    float _radius;
    float _theta;
    float _phi;
};


#endif //FEATHERGL_ORBITCAMERA_H
