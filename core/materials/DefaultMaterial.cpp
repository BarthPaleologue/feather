//
// Created by barth on 19/09/2022.
//

#include "DefaultMaterial.h"

DefaultMaterial::DefaultMaterial() : Material("./assets/shaders/default") {
    setBackFaceCullingEnabled(false);
}
