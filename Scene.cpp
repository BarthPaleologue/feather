//
// Created by barth on 19/09/2022.
//

#include <iostream>
#include "Scene.h"

void Scene::addDrawable(Drawable *drawable) {
    _drawables.push_back(drawable);
}

void Scene::render() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (Drawable* &_drawable: _drawables) {
        _drawable->render();
    }
}
