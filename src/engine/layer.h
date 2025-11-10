#pragma once

#include "state.h"

namespace engine {

template <typename TShared>
class Layer {
public:
    virtual void render(const std::shared_ptr<engine::State<TShared>>& state);

    Layer(const Layer&) = default;
    Layer(Layer&&) = delete;
    Layer& operator=(const Layer&) = default;
    Layer& operator=(Layer&&) = delete;
    virtual ~Layer();
};

}
