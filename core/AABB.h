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

    AABB() : _min(glm::vec3(0)), _max(glm::vec3(0)) {}

    AABB(glm::vec3 min, glm::vec3 max) : _min(min), _max(max) {}

    bool intersects(const AABB &other) const {
        return (_min.x <= other._max.x && _max.x >= other._min.x) &&
               (_min.y <= other._max.y && _max.y >= other._min.y) &&
               (_min.z <= other._max.z && _max.z >= other._min.z);
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

            _min.x = std::min(transformedPosition.x, _min.x);
            _min.y = std::min(transformedPosition.y, _min.y);
            _min.z = std::min(transformedPosition.z, _min.z);

            _max.x = std::max(transformedPosition.x, _max.x);
            _max.y = std::max(transformedPosition.y, _max.y);
            _max.z = std::max(transformedPosition.z, _max.z);
        }
    }
};

#endif //FEATHERGL_AABB_H
