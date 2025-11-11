#include "application.h"

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

#include "engine/rigging.h"
#include "engine/shutdown_glfw_imgui.h"
#include "engine/startup_glfw_imgui.h"
#include "imgui.h"

class DemoLayer : public engine::RenderStep {
public:
    DemoLayer()
        : show_demo_window_(true),
          clear_color_{0.45F, 0.55F, 0.60F, 1.0F},
          counter_(0),
          slider_value_(0.0F) {}

    void onRender(const std::shared_ptr<engine::Rigging>& rigging) override {
        ImGuiIO& imguiIO = ImGui::GetIO();

        if (show_demo_window_) {
            ImGui::ShowDemoWindow(&show_demo_window_);
        }

        ImGui::Begin("Hello, world!");
        ImGui::TextUnformatted("This is some useful text.");
        ImGui::Checkbox("Demo Window", &show_demo_window_);
        ImGui::SliderFloat("float", &slider_value_, 0.0F, 1.0F);
        ImGui::ColorEdit3("clear color", &clear_color_.x);

        if (ImGui::Button("Button")) {
            counter_++;
        }
        ImGui::SameLine();
        ImGui::TextUnformatted(
            ("counter = " + std::to_string(counter_)).c_str());

        ImGui::TextUnformatted(("Application average " +
                                std::to_string(1000.0F / imguiIO.Framerate) +
                                " ms/frame (" +
                                std::to_string(imguiIO.Framerate) + " FPS)")
                                   .c_str());
        ImGui::End();

        int displayWidth = 0;
        int displayHeight = 0;
        glfwGetFramebufferSize(rigging->window.get(), &displayWidth,
                               &displayHeight);
        glViewport(0, 0, displayWidth, displayHeight);
        glClearColor(clear_color_.x * clear_color_.w,
                     clear_color_.y * clear_color_.w,
                     clear_color_.z * clear_color_.w, clear_color_.w);
        glClear(GL_COLOR_BUFFER_BIT);
    }

private:
    bool show_demo_window_;
    ImVec4 clear_color_;
    int counter_;
    float slider_value_;
};

void Application::run() {
    engine_.addStartupStep(std::make_shared<surface::StartupGlfwImGui>(
        "Example App", 1280, 720, true));
    engine_.addShutdownStep(std::make_shared<surface::ShutdownGlfwImGui>());

    engine_.addRenderStep(std::make_shared<DemoLayer>());

    engine_.run();
}
