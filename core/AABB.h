//
// Created by barth on 04/02/24.
//

#ifndef FEATHERGL_AABB_H
#define FEATHERGL_AABB_H

#include <glm/glm.hpp>
#include <vector>
#include "glad/glad.h"
#include "VertexData.h"

class AABB {
public:
    glm::vec3 _min;
    glm::vec3 _max;

    AABB() : _min(std::numeric_limits<float>::max()), _max(-_min) {}

    AABB(glm::vec3 min, glm::vec3 max) : _min(min), _max(max) {}

    bool intersects(const AABB &other) const {
        return (_min.x <= other._max.x && _max.x >= other._min.x) &&
               (_min.y <= other._max.y && _max.y >= other._min.y) &&
               (_min.z <= other._max.z && _max.z >= other._min.z);
    }

    static AABB* intersection(const AABB *a, const AABB *b) {
        if (!a->intersects(*b)) {
            return nullptr;
        }

        glm::vec3 min = glm::max(a->_min, b->_min);
        glm::vec3 max = glm::min(a->_max, b->_max);

        return new AABB(min, max);
    }

    bool intersectsTriangle(glm::vec3 t0, glm::vec3 t1, glm::vec3 t2) const {
        AABB triangleAABB{};
        triangleAABB.expand(t0);
        triangleAABB.expand(t1);
        triangleAABB.expand(t2);

        return intersects(triangleAABB);
    }

    bool intersectsRay(glm::vec3 rayOrigin, glm::vec3 rayDirection) const {
        float tmin = (this->_min.x - rayOrigin.x) / rayDirection.x;
        float tmax = (this->_max.x - rayOrigin.x) / rayDirection.x;

        if (tmin > tmax) std::swap(tmin, tmax);

        float tymin = (this->_min.y - rayOrigin.y) / rayDirection.y;
        float tymax = (this->_max.y - rayOrigin.y) / rayDirection.y;

        if (tymin > tymax) std::swap(tymin, tymax);

        if ((tmin > tymax) || (tymin > tmax)) return false;

        if (tymin > tmin) tmin = tymin;
        if (tymax < tmax) tmax = tymax;

        float tzmin = (this->_min.z - rayOrigin.z) / rayDirection.z;
        float tzmax = (this->_max.z - rayOrigin.z) / rayDirection.z;

        if (tzmin > tzmax) std::swap(tzmin, tzmax);

        return (tmin <= tzmax) && (tzmin <= tmax);
    }

    bool contains(const glm::vec3 &point) const {
        return (point.x >= _min.x && point.x <= _max.x) &&
               (point.y >= _min.y && point.y <= _max.y) &&
               (point.z >= _min.z && point.z <= _max.z);
    }

    glm::vec3 center() const {
        return (_min + _max) * 0.5f;
    }

    glm::vec3 size() const {
        return _max - _min;
    }

    void expand(const glm::vec3 &point) {
        _min = glm::min(_min, point);
        _max = glm::max(_max, point);
    }

    void expand(const AABB &other) {
        _min = glm::min(_min, other._min);
        _max = glm::max(_max, other._max);
    }

    void expand(float amount) {
        _min -= glm::vec3(amount);
        _max += glm::vec3(amount);
    }

    void set(const glm::vec3 &min, const glm::vec3 &max) {
        this->_min = min;
        this->_max = max;
    }

    void updateWithVertexData(VertexData &vertexData, glm::mat4 worldMatrix) {
        // convert all positions to world space
        std::vector<GLfloat> positions = vertexData.positions;
        _min = glm::vec3(std::numeric_limits<float>::max());
        _max = -_min;

        for (int i = 0; i < positions.size(); i += 3) {
            glm::vec4 position = glm::vec4(positions[i], positions[i + 1], positions[i + 2], 1.0f);
            glm::vec4 transformedPosition = worldMatrix * position;

            expand(glm::vec3(transformedPosition));
        }

        expand(0.3f);
    }

    float volume() const {
        glm::vec3 s = size();
        return s.x * s.y * s.z;
    }
};

#endif //FEATHERGL_AABB_H
