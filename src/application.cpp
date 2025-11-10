#include "application.h"

#include <memory>

#include "imgui.h"

class DemoLayer : public engine::Layer<SharedState> {
public:
    void render(
        const std::shared_ptr<engine::State<SharedState>>& state) override {
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

        const std::string counterText = "counter = " + std::to_string(counter_);
        ImGui::TextUnformatted(counterText.c_str());

        const std::string fpsText =
            "Application average " +
            std::to_string(1000.0F / imguiIO.Framerate) + " ms/frame (" +
            std::to_string(imguiIO.Framerate) + " FPS)";
        ImGui::TextUnformatted(fpsText.c_str());

        ImGui::End();

        int displayWidth = 0;
        int displayHeight = 0;
        glfwGetFramebufferSize(state->window.get(), &displayWidth,
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
    engine_.initialize(
        std::make_shared<engine::Settings>(1280, 720, "Example App", true),
        std::make_shared<SharedState>());
    engine_.run();
}

void Application::quit() { engine::Engine<SharedState>::stop(); }
