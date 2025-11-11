#pragma once

#include "state.h"
#include "step.h"

namespace engine {

template <typename TState>
    requires std::derived_from<TState, engine::State>
class ShutdownStep : public virtual engine::Step<TState> {
public:
    virtual void onShutdown(const std::shared_ptr<TState>& state) = 0;
};

}
