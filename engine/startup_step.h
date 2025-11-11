#pragma once

#include "step.h"

namespace engine {

template <typename TState>
    requires std::derived_from<TState, engine::State>
class StartupStep : public virtual engine::Step<TState> {
public:
    virtual void onStartup() = 0;
};

}
