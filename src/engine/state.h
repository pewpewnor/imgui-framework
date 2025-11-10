#pragma once

#include <memory>

#include "glfw_bindings.h"
#include "settings.h"

namespace engine {

template <typename TShared>
class State {
public:
    engine::Settings settings;
    TShared shared;
    std::shared_ptr<glfw::Window> window;

    State(const Settings& settings, const TShared& shared)
        : settings(settings), shared(shared) {}
};

}
