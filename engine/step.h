#pragma once

#include "state.h"

namespace engine {

template <typename TState>
    requires std::derived_from<TState, engine::State>
class Step {
public:
    Step(const Step&) = default;
    Step(Step&&) = default;
    Step& operator=(const Step&) = default;
    Step& operator=(Step&&) = default;
    virtual ~Step() = default;

protected:
    Step(const std::shared_ptr<TState>& state) : state(state) {}

    std::shared_ptr<TState> state;
};

}
