#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cassert>
#include <memory>
#include <vector>

#include "glfw_bindings.h"
#include "render_stage.h"
#include "rigging.h"
#include "shutdown_stage.h"
#include "startup_stage.h"

namespace engine {

class Engine {
public:
    void run() {
        assert(startupStages_.size() > 0 &&
               "GLFW and ImGui needs to have a startup");
        assert(shutdownStages_.size() > 0 &&
               "GLFW and ImGui needs to have a shutdown");

        startup();
        continouslyRenderFrames();
        shutdown();
    }

    void addStartupStage(const std::shared_ptr<engine::StartupStage>& stage) {
        startupStages_.push_back(stage);
    }

    void addRenderStage(const std::shared_ptr<engine::RenderLayer>& stage) {
        renderStages_.push_back(stage);
    }

    void addShutdownStage(const std::shared_ptr<engine::ShutdownStage>& stage) {
        shutdownStages_.push_back(stage);
    }

    void requestStop() { rigging_->stopSignal = true; }

private:
    std::vector<std::shared_ptr<engine::StartupStage>> startupStages_;
    std::vector<std::shared_ptr<engine::RenderLayer>> renderStages_;
    std::vector<std::shared_ptr<engine::ShutdownStage>> shutdownStages_;
    std::shared_ptr<engine::Rigging> rigging_ =
        std::make_shared<engine::Rigging>();

    void startup() {
        for (const auto& stage : startupStages_) {
            stage->onStartup(rigging_);
        }
        startupStages_.clear();
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
        renderStages_.clear();
    }

    void shutdown() {
        for (const auto& stage : shutdownStages_) {
            stage->onShutdown(rigging_);
        }
        shutdownStages_.clear();
        rigging_.reset();
    }

    void renderFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (const auto& stage : renderStages_) {
            stage->onRender(rigging_);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfw::swapBuffers(rigging_->window);
    }
};

}
