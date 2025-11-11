#pragma once

#include "glfw_bindings.h"

namespace engine {

struct State {
    std::shared_ptr<glfw::Window> glfwWindow;
    bool engineStopSignal = false;
};

}
