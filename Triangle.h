//
// Created by barth on 26/09/2022.
//

#ifndef FEATHERGL_TRIANGLE_H
#define FEATHERGL_TRIANGLE_H

#include "Drawable.h"

class Triangle: public Drawable {
public:
    Triangle(const char* name, float x, float y, float z);
};


#endif //FEATHERGL_TRIANGLE_H
