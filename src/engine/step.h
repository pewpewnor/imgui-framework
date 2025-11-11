#pragma once

#include <concepts>

#include "state.h"
namespace engine {

template <typename TState>
    requires std::derived_from<TState, engine::State>
class Step {
public:
    Step(const std::shared_ptr<TState>& state) : state(state) {}

    Step(const Step&) = default;
    Step(Step&&) = delete;
    Step& operator=(const Step&) = default;
    Step& operator=(Step&&) = delete;
    virtual ~Step() = default;

protected:
    std::shared_ptr<TState> state;
};

}
