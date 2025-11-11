#pragma once

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "shutdown_step.h"
#include "state.h"

namespace surface {

template <typename TState>
    requires std::derived_from<TState, engine::State>
class ShutdownGlfwImGui : public engine::ShutdownStep<TState> {
public:
    explicit ShutdownGlfwImGui(const std::shared_ptr<TState>& state)
        : engine::Step<TState>(state) {}

    void onShutdown(const std::shared_ptr<TState>& state) override {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
        state->window.reset();
        glfw::terminate();
    }
};

}
