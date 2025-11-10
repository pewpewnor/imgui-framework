#pragma once

#include <memory>

#include "glfw_bindings.h"
#include "settings.h"

namespace engine {

template <typename TShared>
class State {
public:
    engine::Settings settings;
    std::shared_ptr<glfw::Window> window;
    TShared shared;

    State(const Settings& settings, const TShared& shared)
        : settings(settings), shared(shared) {}
};

}
