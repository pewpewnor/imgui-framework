#pragma once

#include "state.h"
#include "step.h"

namespace engine {

template <typename TState>
    requires std::derived_from<TState, engine::State>
class RenderStep : public virtual engine::Step<TState> {
public:
    virtual void onRender() = 0;
};

}
