//
// Created by barth on 19/09/2022.
//

#ifndef FEATHERGL_VECTOR3_H
#define FEATHERGL_VECTOR3_H

template<typename T>
class Vector3 {
public:
    Vector3(T x, T y, T z);

private:
    T _x;
    T _y;
    T _z;
};

template<typename T>
Vector3<T>::Vector3(T x, T y, T z): _x(x), _y(y), _z(z) {}

#endif //FEATHERGL_VECTOR3_H