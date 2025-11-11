#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cassert>
#include <memory>
#include <vector>

#include "glfw_bindings.h"
#include "render_step.h"
#include "shutdown_step.h"
#include "startup_step.h"
#include "state.h"

namespace engine {

template <typename TState>
    requires std::derived_from<TState, engine::State>
class Engine {
public:
    Engine(const std::shared_ptr<TState>& state) : state_(state) {};

    void run() {
        assert(startupSteps_.size() > 0 &&
               "GLFW and ImGui needs to have a startup");
        assert(shutdownSteps_.size() > 0 &&
               "GLFW and ImGui needs to have a shutdown");

        startup();
        continouslyRenderFrames();
        shutdown();
    }

    void addStartupStep(
        const std::shared_ptr<engine::StartupStep<TState>>& step) {
        startupSteps_.push_back(step);
    }

    void addRenderStep(
        const std::shared_ptr<engine::RenderStep<TState>>& step) {
        renderSteps_.push_back(step);
    }

    void addShutdownStep(
        const std::shared_ptr<engine::ShutdownStep<TState>>& step) {
        shutdownSteps_.push_back(step);
    }

    void requestStop() { state_->engineStopSignal = true; }

private:
    std::shared_ptr<TState> state_;
    std::vector<std::shared_ptr<engine::StartupStep<TState>>> startupSteps_;
    std::vector<std::shared_ptr<engine::RenderStep<TState>>> renderSteps_;
    std::vector<std::shared_ptr<engine::ShutdownStep<TState>>> shutdownSteps_;

    void startup() {
        for (const auto& step : startupSteps_) {
            step->onStartup();
        }
        startupSteps_.clear();
    }

    void continouslyRenderFrames() {
        while (!glfw::windowShouldClose(state_->glfwWindow) &&
               !state_->engineStopSignal) {
            glfw::pollEvents();
            if (glfw::windowAttributeIsError(state_->glfwWindow)) {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }
            renderFrame();
        }
        renderSteps_.clear();
    }

    void shutdown() {
        for (const auto& step : shutdownSteps_) {
            step->onShutdown();
        }
        shutdownSteps_.clear();
        state_.reset();
    }

    void renderFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (const auto& step : renderSteps_) {
            step->onRender();
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfw::swapBuffers(state_->glfwWindow);
    }
};

}
