//
// Created by barth on 27/09/2022.
//

#ifndef FEATHERGL_LIGHT_H
#define FEATHERGL_LIGHT_H


#include <string>

class Light {
public:
    explicit Light(const char* name);

private:
    std::string _name;
};


#endif //FEATHERGL_LIGHT_H
