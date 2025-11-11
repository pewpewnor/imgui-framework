#include "shutdown_glfw_imgui.h"

#include <memory>

#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "rigging.h"

void surface::ShutdownGlfwImGui::onShutdown(
    const std::shared_ptr<engine::Rigging>& rigging) {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    rigging->window.reset();
    glfw::terminate();
}
