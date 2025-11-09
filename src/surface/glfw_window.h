#pragma once

#include "glfw.h"

namespace surface {

class GlfwWindow {
public:
    GlfwWindow(int width, int height, const std::string& title);

    [[nodiscard]] glfw::Window* get() const;

    ~GlfwWindow();

    GlfwWindow(const GlfwWindow&) = delete;
    GlfwWindow(GlfwWindow&&) = delete;
    GlfwWindow& operator=(const GlfwWindow&) = delete;
    GlfwWindow& operator=(GlfwWindow&&) = delete;

private:
    glfw::Window* window_;
};

}
