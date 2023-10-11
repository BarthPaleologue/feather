//
// Created by barth on 11/10/23.
//

#ifndef FEATHERGL_CLOTH_H
#define FEATHERGL_CLOTH_H

#include "PhysicsBody.h"

class Cloth : public PhysicsBody {
public:
    Cloth(Mesh *mesh, float mass) : PhysicsBody(mesh, mass) {};
};

#endif //FEATHERGL_CLOTH_H
