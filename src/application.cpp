#include "application.h"

#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <string>

#include "imgui.h"

// DemoLayer derived from engine::Layer<SharedState>
class DemoLayer : public engine::Layer<SharedState> {
public:
    DemoLayer()
        : show_demo_window_(true),
          clear_color_{0.45F, 0.55F, 0.60F, 1.0F},
          counter_(0),
          slider_value_(0.0F) {}

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

// Application methods
void Application::run() {
    // Create Settings and SharedState
    auto settings =
        std::make_shared<engine::Settings>(1280, 720, "Example App", true);
    auto shared = std::make_shared<SharedState>();

    // Create engine instance with settings and shared state
    engine_ = std::make_unique<engine::Engine<SharedState>>(settings, shared);

    // Initialize the engine
    engine_->initialize();

    // Add DemoLayer
    engine_->addLayer(std::make_shared<DemoLayer>());

    // Start main loop
    engine_->run();
}

void Application::quit() {
    engine::Engine<SharedState>::stop();  // Call static method directly
}
