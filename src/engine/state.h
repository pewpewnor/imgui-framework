#pragma once

#include <memory>

#include "glfw_bindings.h"
#include "settings.h"

namespace engine {

template <typename TShared>
class State {
public:
    std::shared_ptr<engine::Settings> settings;
    std::shared_ptr<glfw::Window> window;
    std::shared_ptr<TShared> shared;

    State(const std::shared_ptr<engine::Settings>& settings,
          const std::shared_ptr<glfw::Window>& window,
          const std::shared_ptr<TShared>& shared)
        : settings(settings), window(window), shared(shared) {}
};

}
