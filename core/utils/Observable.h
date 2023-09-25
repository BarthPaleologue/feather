//
// Created by barth on 25/09/23.
//

#ifndef FEATHERGL_OBSERVABLE_H
#define FEATHERGL_OBSERVABLE_H


#include <vector>
#include <algorithm>
#include <functional>

template<typename T>
class Observable {
public:
    Observable() = default;

    void add(std::function<void(T)> observer) {
        observers.push_back(observer);
    }

    void addOnce(std::function<void(T)> observer) {
        observersOnce.push_back(observer);
    }

    void notifyObservers() {
        for (auto &observer: observers) {
            observer->update();
        }
        for (auto &observer: observersOnce) {
            observer->update();
        }
        observersOnce.clear();
    }

private:
    // vector of lambda taking T as parameter
    std::vector<std::function<void(T)>> observers;
    std::vector<std::function<void(T)>> observersOnce;
};


#endif //FEATHERGL_OBSERVABLE_H
