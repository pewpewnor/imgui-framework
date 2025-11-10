#pragma once

#include "state.h"

namespace engine {

template <typename TShared>
class Layer {
public:
    Layer() = default;

    virtual void render(
        const std::shared_ptr<engine::State<TShared>>& state) = 0;

    Layer(const Layer&) = default;
    Layer(Layer&&) = delete;
    Layer& operator=(const Layer&) = default;
    Layer& operator=(Layer&&) = delete;
    virtual ~Layer() = default;
};

}
