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
};

}
