//
// Created by barth on 08/01/24.
//

#ifndef FEATHERGL_AABB_H
#define FEATHERGL_AABB_H

#include <glm/vec3.hpp>
#include <glm/common.hpp>
#include <vector>

class AABB {
public:
    AABB(glm::vec3 min, glm::vec3 max) : _min(min), _max(max) {};

    glm::vec3 min() {
        return _min;
    }

    glm::vec3 max() {
        return _max;
    }

    glm::vec3 center() {
        return (_min + _max) / 2.0f;
    }

    glm::vec3 size() {
        return _max - _min;
    }

    void setMin(glm::vec3 min) {
        _min = min;
    }

    void setMax(glm::vec3 max) {
        _max = max;
    }

    void setMinMaxFromPositions(std::vector<float> positions) {
        _min = glm::vec3(positions[0], positions[1], positions[2]);
        _max = glm::vec3(positions[0], positions[1], positions[2]);
        for (int i = 3; i < positions.size(); i += 3) {
            _min = glm::min(_min, glm::vec3(positions[i], positions[i + 1], positions[i + 2]));
            _max = glm::max(_max, glm::vec3(positions[i], positions[i + 1], positions[i + 2]));
        }
    }

    bool contains(glm::vec3 point) const {
        return point.x >= _min.x && point.x <= _max.x &&
               point.y >= _min.y && point.y <= _max.y &&
               point.z >= _min.z && point.z <= _max.z;
    }

    bool intersects(AABB &other) {
        return _min.x <= other._max.x && _max.x >= other._min.x &&
               _min.y <= other._max.y && _max.y >= other._min.y &&
               _min.z <= other._max.z && _max.z >= other._min.z;
    }

    AABB intersection(AABB &other) {
        glm::vec3 min = glm::max(_min, other._min);
        glm::vec3 max = glm::min(_max, other._max);
        return {min, max};
    }

private:
    glm::vec3 _min;
    glm::vec3 _max;
};

#endif //FEATHERGL_AABB_H
