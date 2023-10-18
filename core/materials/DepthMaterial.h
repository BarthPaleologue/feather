//
// Created by barth on 18/10/23.
//

#ifndef FEATHERGL_DEPTHMATERIAL_H
#define FEATHERGL_DEPTHMATERIAL_H

#include "Material.h"

class DepthMaterial : public Material {
public:
    DepthMaterial() : Material("./assets/shaders/depth") {
        setBackFaceCullingEnabled(false);
    };
};

#endif //FEATHERGL_DEPTHMATERIAL_H
