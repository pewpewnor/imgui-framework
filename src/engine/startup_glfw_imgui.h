#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

#include "startup_step.h"

namespace surface {

template <typename TState>
    requires std::derived_from<TState, engine::State>
class StartupGlfwImGui : public engine::StartupStep<TState> {
public:
    StartupGlfwImGui(const std::shared_ptr<TState>& state,
                     const std::string& title, int width, int height,
                     bool vsync)
        : engine::Step<TState>(state),
          title_(title),
          width_(width),
          height_(height),
          vsync_(vsync) {}

    void onStartup(const std::shared_ptr<TState>& state) override {
        initializeGlfw(state);
        initializeImGui(state);
    }

private:
    std::string title_;
    int width_ = 1280;
    int height_ = 720;
    bool vsync_ = true;
    float scale_ = 1;

    void initializeGlfw(const std::shared_ptr<TState>& state) {
        glfw::setErrorCallback([](int errorCode, const char* description) {
            std::cerr << "GLFW Error " << errorCode << ": " << description
                      << std::endl;
        });

        if (!glfw::init()) {
            throw std::runtime_error("failed to initialize GLFW");
        }

        scale_ = ImGui_ImplGlfw_GetContentScaleForMonitor(
            glfw::getPrimaryMonitor().get());

        state->window = glfw::createWindow(
            static_cast<int>(static_cast<float>(width_) * scale_),
            static_cast<int>(static_cast<float>(height_) * scale_), title_);

        glfw::makeContextCurrent(state->window);
        glfw::switchVsync(vsync_);
    }

    void initializeImGui(const std::shared_ptr<TState>& state) const {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& ioContext = ImGui::GetIO();
        ioContext.ConfigFlags = static_cast<int>(
            static_cast<unsigned int>(ioContext.ConfigFlags) |
            static_cast<unsigned int>(ImGuiConfigFlags_NavEnableKeyboard));

        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(scale_);

        if (!ImGui_ImplGlfw_InitForOpenGL(state->window.get(), true)) {
            throw std::runtime_error("failed to initialize ImGui GLFW backend");
        }

        if (!ImGui_ImplOpenGL3_Init(glfw::glslVersion().c_str())) {
            ImGui_ImplGlfw_Shutdown();
            throw std::runtime_error(
                "failed to initialize ImGui OpenGL3 backend");
        }
    }
};

}
