//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_SCENE_H
#define FEATHERGL_SCENE_H


#include "../drawables/Drawable.h"
#include "cameras/Camera.h"
#include "lights/PointLight.h"

class Scene {
public:
    void addDrawable(Drawable &drawable);
    void render(Camera &camera, PointLight &light);

private:
    std::vector<Drawable> _drawables;
};


#endif //FEATHERGL_SCENE_H
