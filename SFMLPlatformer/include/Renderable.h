#pragma once

#include <iostream>
#include <functional>

struct Renderable {
    int layer;
    std::function<void()> renderFunc;

    Renderable(int layer, const std::function<void()>& renderFunc) : layer(layer), renderFunc(renderFunc) {}

    bool operator<(const Renderable& other) const {
        return layer < other.layer;
    }
};