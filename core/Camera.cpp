//
// Created by barth on 26/09/2022.
//

#include "Camera.h"

Camera::Camera(GLFWwindow *window) {
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    setAspectRatio(static_cast<float>(width)/static_cast<float>(height));

    setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    setNear(0.1);
    setFar(80.1);
}
