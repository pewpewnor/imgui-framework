#include "startup_glfw_imgui.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <iostream>

surface::StartupGlfwImGui::StartupGlfwImGui(const std::string& title, int width,
                                            int height, bool vsync)
    : title_(title), width_(width), height_(height), vsync_(vsync) {}

void surface::StartupGlfwImGui::onStartup(
    const std::shared_ptr<engine::Rigging>& rigging) {
    initializeGlfw(rigging);
    initializeImGui(rigging);
}

void surface::StartupGlfwImGui::initializeGlfw(
    const std::shared_ptr<engine::Rigging>& rigging) {
    glfw::setErrorCallback([](int errorCode, const char* description) {
        std::cerr << "GLFW Error " << errorCode << ": " << description
                  << std::endl;
    });

    if (!glfw::init()) {
        throw std::runtime_error("failed to initialize GLFW");
    }

    scale_ = ImGui_ImplGlfw_GetContentScaleForMonitor(
        glfw::getPrimaryMonitor().get());

    rigging->window = glfw::createWindow(
        static_cast<int>(static_cast<float>(width_) * scale_),
        static_cast<int>(static_cast<float>(height_) * scale_), title_);

    glfw::makeContextCurrent(rigging->window);
    glfw::switchVsync(vsync_);
}

void surface::StartupGlfwImGui::initializeImGui(
    const std::shared_ptr<engine::Rigging>& rigging) const {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiIO& ioContext = ImGui::GetIO();
    ioContext.ConfigFlags = static_cast<int>(
        static_cast<unsigned int>(ioContext.ConfigFlags) |
        static_cast<unsigned int>(ImGuiConfigFlags_NavEnableKeyboard));

    ImGui::StyleColorsDark();
    ImGui::GetStyle().ScaleAllSizes(scale_);

    if (!ImGui_ImplGlfw_InitForOpenGL(rigging->window.get(), true)) {
        throw std::runtime_error("failed to initialize ImGui GLFW backend");
    }

    if (!ImGui_ImplOpenGL3_Init(glfw::glslVersion().c_str())) {
        ImGui_ImplGlfw_Shutdown();
        throw std::runtime_error("failed to initialize ImGui OpenGL3 backend");
    }
}
