#pragma once
#include <imgui.h>

#include <memory>

#include "surface/glfw_context.h"
#include "surface/glfw_window.h"
#include "surface/imgui_context.h"

class Application {
public:
    Application();

    ~Application();

    bool initialize();

    void run();

    void shutdown();

    Application(const Application&) = delete;
    Application(Application&&) = delete;
    Application& operator=(const Application&) = delete;
    Application& operator=(Application&&) = delete;

private:
    std::unique_ptr<surface::GlfwContext> glfw_context_;
    std::unique_ptr<surface::GlfwWindow> window_;
    std::unique_ptr<surface::ImGuiContext> imgui_context_;

    bool show_demo_window_;
    bool is_initialized_;
    ImVec4 clear_color_;
    int counter_;
    float slider_value_;
    float scale_;

    bool initializeImGui() const;

    void renderFrame();

    void renderUI();
};
