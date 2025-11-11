#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

#include "shutdown_step.h"
#include "startup_step.h"
#include "step.h"

namespace engine {

template <typename TState>
    requires std::derived_from<TState, engine::State>
class GlfwImguiSurface : public engine::StartupStep<TState>,
                         public engine::ShutdownStep<TState> {
public:
    GlfwImguiSurface(const std::shared_ptr<TState>& state,
                     const std::string& title, int width, int height,
                     bool vsync)
        : engine::Step<TState>(state),
          title_(title),
          width_(width),
          height_(height),
          vsync_(vsync) {}

    void onStartup() override {
        initializeGlfw();
        initializeImgui();
    }

    void onShutdown() override {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        this->state->glfwWindow.reset();
        glfw::terminate();
    }

private:
    std::string title_;
    int width_ = 1280;
    int height_ = 720;
    bool vsync_ = true;
    float scale_ = 1;

    void initializeGlfw() {
        glfw::setErrorCallback([](int errorCode, const char* description) {
            std::cerr << "GLFW Error " << errorCode << ": " << description
                      << std::endl;
        });

        if (!glfw::init()) {
            throw std::runtime_error("failed to initialize GLFW");
        }

        scale_ = ImGui_ImplGlfw_GetContentScaleForMonitor(
            glfw::getPrimaryMonitor().get());

        this->state->glfwWindow = glfw::createWindow(
            static_cast<int>(static_cast<float>(width_) * scale_),
            static_cast<int>(static_cast<float>(height_) * scale_), title_);

        glfw::makeContextCurrent(this->state->glfwWindow);
        glfw::switchVsync(vsync_);
    }

    void initializeImgui() const {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& ioContext = ImGui::GetIO();
        ioContext.ConfigFlags = static_cast<int>(
            static_cast<unsigned int>(ioContext.ConfigFlags) |
            static_cast<unsigned int>(ImGuiConfigFlags_NavEnableKeyboard));

        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(scale_);

        if (!ImGui_ImplGlfw_InitForOpenGL(this->state->glfwWindow.get(),
                                          true)) {
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
