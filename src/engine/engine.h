#pragma once

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

#include "node.h"

namespace engine {

template <typename TState>
    requires std::derived_from<TState, engine::State>
class Engine {
public:
    void initialize(const std::shared_ptr<Node<TState>>& rootNode,
                    const std::shared_ptr<TState>& state,
                    const std::string& title, int width, int height,
                    bool vsync) {
        rootNode_ = rootNode;
        state_ = state;
        float scale = initializeGlfw(title, width, height, vsync);
        initializeImgui(scale);
    }

    void run() {
        continouslyRenderFrames();
        shutdown();
    }

    void requestStop() { state_->engineStopSignal = true; }

private:
    std::shared_ptr<TState> state_;
    std::shared_ptr<Node<TState>> rootNode_;

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
    }

    void shutdown() {
        rootNode_.reset();
        state_.reset();
    }

    void renderFrame() {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        rootNode_->render();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfw::swapBuffers(state_->glfwWindow);
    }

    float initializeGlfw(const std::string& title, int width, int height,
                         bool vsync) {
        glfw::setErrorCallback([](int errorCode, const char* description) {
            std::cerr << "GLFW Error " << errorCode << ": " << description
                      << std::endl;
        });

        if (!glfw::init()) {
            throw std::runtime_error("failed to initialize GLFW");
        }

        float scale = ImGui_ImplGlfw_GetContentScaleForMonitor(
            glfw::getPrimaryMonitor().get());

        state_->glfwWindow = glfw::createWindow(
            static_cast<int>(static_cast<float>(width) * scale),
            static_cast<int>(static_cast<float>(height) * scale), title);

        glfw::makeContextCurrent(state_->glfwWindow);
        glfw::switchVsync(vsync);
        return scale;
    }

    void initializeImgui(float scale) const {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& ioContext = ImGui::GetIO();
        ioContext.ConfigFlags = static_cast<int>(
            static_cast<unsigned int>(ioContext.ConfigFlags) |
            static_cast<unsigned int>(ImGuiConfigFlags_NavEnableKeyboard));

        ImGui::StyleColorsDark();
        ImGui::GetStyle().ScaleAllSizes(scale);

        if (!ImGui_ImplGlfw_InitForOpenGL(state_->glfwWindow.get(), true)) {
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
