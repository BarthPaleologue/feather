//
// Created by barth on 05/02/24.
//

#ifndef FEATHERGL_GLOBALVOLUMECONSTRAINT_H
#define FEATHERGL_GLOBALVOLUMECONSTRAINT_H

#include <glm/geometric.hpp>
#include <iostream>
#include "Constraint.h"
#include "glad/glad.h"

class GlobalVolumeConstraint : public Constraint {
public:
    GlobalVolumeConstraint(std::vector<std::shared_ptr<Particle>> particles, std::vector<GLint> indices,
                           float pressure, float compliance) : Constraint(
            particles, compliance, EQUALITY), _pressure(pressure), _indices(indices) {

        float volume = 0;
        for(int i = 0; i < _indices.size(); i+=3) {
            glm::vec3 t0 = _particles[_indices[i]]->position;
            glm::vec3 t1 = _particles[_indices[i+1]]->position;
            glm::vec3 t2 = _particles[_indices[i+2]]->position;

            volume += glm::dot(t0, glm::cross(t1, t2)) / 6.0f;
        }

        _restVolume = volume;
    };

    void setPressure(float pressure) {
        _pressure = pressure;
    }

    float pressure() const {
        return _pressure;
    }

    float evaluate() const override {
        float volume = 0;
        for(int i = 0; i < _indices.size(); i+=3) {
            glm::vec3 t0 = _particles[_indices[i]]->predictedPosition;
            glm::vec3 t1 = _particles[_indices[i+1]]->predictedPosition;
            glm::vec3 t2 = _particles[_indices[i+2]]->predictedPosition;

            volume += glm::dot(t0, glm::cross(t1, t2)) / 6.0f;
        }

        return volume - _restVolume * _pressure;
    }

private:
    void computeGradient() override {
        _gradient = Eigen::MatrixXf::Zero(3, _particles.size());
        for(int i = 0; i < _indices.size(); i+=3) {
            glm::vec3 p0 = _particles[_indices[i]]->predictedPosition;
            glm::vec3 p1 = _particles[_indices[i+1]]->predictedPosition;
            glm::vec3 p2 = _particles[_indices[i+2]]->predictedPosition;

            glm::vec3 g1 = glm::cross(p1, p2) / 6.0f;
            glm::vec3 g2 = glm::cross(p2, p0) / 6.0f;
            glm::vec3 g3 = glm::cross(p0, p1) / 6.0f;

            _gradient.col(_indices[i]) += Eigen::Vector3f(g1.x, g1.y, g1.z);
            _gradient.col(_indices[i+1]) += Eigen::Vector3f(g2.x, g2.y, g2.z);
            _gradient.col(_indices[i+2]) += Eigen::Vector3f(g3.x, g3.y, g3.z);
        }
    }

    void recomputeTargetValue() override {
        float volume = 0;
        for(int i = 0; i < _indices.size(); i+=3) {
            glm::vec3 t0 = _particles[_indices[i]]->predictedPosition;
            glm::vec3 t1 = _particles[_indices[i+1]]->predictedPosition;
            glm::vec3 t2 = _particles[_indices[i+2]]->predictedPosition;

            volume += glm::dot(t0, glm::cross(t1, t2)) / 6.0f;
        }

        _restVolume = volume;
    }

    float _restVolume{};
    float _pressure{};
    std::vector<GLint> _indices;
};

#endif //FEATHERGL_GLOBALVOLUMECONSTRAINT_H