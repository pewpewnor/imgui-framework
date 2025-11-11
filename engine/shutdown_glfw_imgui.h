#pragma once

#include "step.h"

namespace surface {

template <typename TState>
    requires std::derived_from<TState, engine::State>
class ShutdownGlfwImGui : public engine::Step<TState> {
public:
    explicit ShutdownGlfwImGui(const std::shared_ptr<TState>& state)
        : engine::Step<TState>(state) {}
};

}
