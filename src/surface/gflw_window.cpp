#include <imgui_impl_glfw.h>

#include <stdexcept>

#include "glfw.h"
#include "glfw_window.h"

surface::GlfwWindow::GlfwWindow(int width, int height, const std::string& title)
    : window_(glfw::createWindow(width, height, title)) {
    if (window_ == nullptr) {
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);
}

glfw::Window* surface::GlfwWindow::get() const { return window_; }

surface::GlfwWindow::~GlfwWindow() { glfwDestroyWindow(get()); }
