#pragma once

#include "glfw_window.h"

namespace surface {

class ImGuiContext {
public:
    ImGuiContext(const surface::GlfwWindow& window);

    ~ImGuiContext();

    ImGuiContext(const ImGuiContext&) = delete;
    ImGuiContext& operator=(const ImGuiContext&) = delete;
    ImGuiContext(ImGuiContext&&) = delete;
    ImGuiContext& operator=(ImGuiContext&&) = delete;
};

}
