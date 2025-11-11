#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cassert>
#include <memory>
#include <vector>

#include "glfw_bindings.h"
#include "render_step.h"
#include "rigging.h"
#include "shutdown_step.h"
#include "startup_step.h"

namespace engine {

class Engine {
public:
    void run() {
        assert(startupSteps_.size() > 0 &&
               "GLFW and ImGui needs to have a startup");
        assert(shutdownSteps_.size() > 0 &&
               "GLFW and ImGui needs to have a shutdown");

        startup();
        continouslyRenderFrames();
        shutdown();
    }

    void addStartupStep(const std::shared_ptr<engine::StartupStep>& step) {
        startupSteps_.push_back(step);
    }

    void addRenderStep(const std::shared_ptr<engine::RenderStep>& step) {
        renderSteps_.push_back(step);
    }

    void addShutdownStep(const std::shared_ptr<engine::ShutdownStep>& step) {
        shutdownSteps_.push_back(step);
    }

    void requestStop() { rigging_->stopSignal = true; }

private:
    std::vector<std::shared_ptr<engine::StartupStep>> startupSteps_;
    std::vector<std::shared_ptr<engine::RenderStep>> renderSteps_;
    std::vector<std::shared_ptr<engine::ShutdownStep>> shutdownSteps_;
    std::shared_ptr<engine::Rigging> rigging_ =
        std::make_shared<engine::Rigging>();

    void startup() {
        for (const auto& step : startupSteps_) {
            step->onStartup(rigging_);
        }
        startupSteps_.clear();
    }

    void continouslyRenderFrames() {
        while (!glfw::windowShouldClose(rigging_->window) &&
               !rigging_->stopSignal) {
            glfw::pollEvents();
            if (glfw::windowAttributeIsError(rigging_->window)) {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }
            renderFrame();
        }
        renderSteps_.clear();
    }

    void shutdown() {
        for (const auto& step : shutdownSteps_) {
            step->onShutdown(rigging_);
        }
        shutdownSteps_.clear();
        rigging_.reset();
    }

    void renderFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (const auto& step : renderSteps_) {
            step->onRender(rigging_);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfw::swapBuffers(rigging_->window);
    }
};

}
