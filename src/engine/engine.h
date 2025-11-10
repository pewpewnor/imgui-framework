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
    Engine() = default;

    void initialize(const std::shared_ptr<engine::Settings>& settings,
                    const std::shared_ptr<TShared>& shared) {
        glfw::setErrorCallback([](int errorCode, const char* description) {
            std::cerr << "GLFW Error " << errorCode << ": " << description
                      << std::endl;
        });

        if (!glfw::init()) {
            throw std::runtime_error("failed to initialize GLFW");
        }

        auto glsl_version = glfw::glslVersion();

        float scale = ImGui_ImplGlfw_GetContentScaleForMonitor(
            glfw::getPrimaryMonitor().get());

        std::shared_ptr<glfw::Window> window = glfw::createWindow(
            static_cast<int>(static_cast<float>(settings->width) * scale),
            static_cast<int>(static_cast<float>(settings->height) * scale),
            settings->title);

        glfw::makeContextCurrent(window);
        glfw::switchVsync(settings->vsync);

        state_ =
            std::make_shared<engine::State<TShared>>(settings, window, shared);

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(scale);

        if (!ImGui_ImplGlfw_InitForOpenGL(window.get(), true)) {
            throw std::runtime_error("failed to initialize ImGui GLFW backend");
        }

        if (!ImGui_ImplOpenGL3_Init(glsl_version.c_str())) {
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
