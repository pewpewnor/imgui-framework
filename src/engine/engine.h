#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>
#include <memory>
#include <vector>

#include "glfw_bindings.h"
#include "layer.h"
#include "settings.h"
#include "state.h"

namespace engine {

template <typename TShared>
class Engine {
public:
    Engine(const std::shared_ptr<engine::Settings>& settings,
           const std::shared_ptr<TShared>& shared)
        : state_(std::make_shared<engine::State<TShared>>()) {
        state_->settings = settings;
        state_->shared = shared;
    };

    void initialize() {
        glfw::setErrorCallback([](int errorCode, const char* description) {
            std::cerr << "GLFW Error " << errorCode << ": " << description
                      << std::endl;
        });

        if (!glfw::init()) {
            throw std::runtime_error("failed to initialize GLFW");
        }

        float scale = ImGui_ImplGlfw_GetContentScaleForMonitor(
            glfw::getPrimaryMonitor().get());

        state_->window = glfw::createWindow(
            static_cast<int>(static_cast<float>(state_->settings->width) *
                             scale),
            static_cast<int>(static_cast<float>(state_->settings->height) *
                             scale),
            state_->settings->title);

        glfw::makeContextCurrent(state_->window);
        glfw::switchVsync(state_->settings->vsync);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& ioContext = ImGui::GetIO();
        ioContext.ConfigFlags = static_cast<int>(
            static_cast<unsigned int>(ioContext.ConfigFlags) |
            static_cast<unsigned int>(ImGuiConfigFlags_NavEnableKeyboard));

        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(scale);

        if (!ImGui_ImplGlfw_InitForOpenGL(state_->window.get(), true)) {
            throw std::runtime_error("failed to initialize ImGui GLFW backend");
        }

        if (!ImGui_ImplOpenGL3_Init(glfw::glslVersion().c_str())) {
            ImGui_ImplGlfw_Shutdown();
            throw std::runtime_error(
                "failed to initialize ImGui OpenGL3 backend");
        }
    }

    void run() {
        while (!glfw::windowShouldClose(state_->window)) {
            glfw::pollEvents();
            if (glfw::windowAttributeIsError(state_->window)) {
                ImGui_ImplGlfw_Sleep(10);
                continue;
            }
            renderFrames();
        }
    }

    void addLayer(const std::shared_ptr<engine::Layer<TShared>>& layer) {
        layers_.push_back(layer);
    }

    static void stop() {
        glfw::terminate();
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

private:
    std::vector<std::shared_ptr<engine::Layer<TShared>>> layers_;
    std::shared_ptr<engine::State<TShared>> state_;

    void renderFrames() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        for (const auto& layer : layers_) {
            layer->render(state_);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfw::swapBuffers(state_->window);
    }
};

}
