#pragma once

#include <memory>

#include "state.h"
#include "step.h"

namespace engine {

template <typename TState>
    requires std::derived_from<TState, engine::State>
class StartupStep : public virtual engine::Step<TState> {
public:
    virtual void onStartup(const std::shared_ptr<TState>& state) = 0;
};

}
