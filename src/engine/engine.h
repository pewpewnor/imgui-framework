#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <cassert>
#include <memory>
#include <vector>

#include "glfw_bindings.h"
#include "render_layer.h"
#include "rigging.h"
#include "shutdown_layer.h"
#include "startup_layer.h"

namespace engine {

class Engine {
public:
    void run() {
        assert(startupLayers_.size() > 0 &&
               "GLFW and ImGui needs to have a startup");
        assert(shutdownLayers_.size() > 0 &&
               "GLFW and ImGui needs to have a shutdown");

        startup();
        continouslyRenderFrames();
        shutdown();
    }

    void addStartupLayer(const std::shared_ptr<engine::StartupLayer>& layer) {
        startupLayers_.push_back(layer);
    }

    void addRenderLayer(const std::shared_ptr<engine::RenderLayer>& layer) {
        renderLayers_.push_back(layer);
    }

    void addShutdownLayer(const std::shared_ptr<engine::ShutdownLayer>& layer) {
        shutdownLayers_.push_back(layer);
    }

    void requestStop() { rigging_->stopSignal = true; }

private:
    std::vector<std::shared_ptr<engine::StartupLayer>> startupLayers_;
    std::vector<std::shared_ptr<engine::RenderLayer>> renderLayers_;
    std::vector<std::shared_ptr<engine::ShutdownLayer>> shutdownLayers_;
    std::shared_ptr<engine::Rigging> rigging_ =
        std::make_shared<engine::Rigging>();

    void startup() {
        for (const auto& layer : startupLayers_) {
            layer->execute(rigging_);
        }
        startupLayers_.clear();
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
        renderLayers_.clear();
    }

    void shutdown() {
        for (const auto& layer : shutdownLayers_) {
            layer->execute(rigging_);
        }
        shutdownLayers_.clear();
        rigging_.reset();
    }

    void renderFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (const auto& layer : renderLayers_) {
            layer->render(rigging_);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfw::swapBuffers(rigging_->window);
    }
};

}
