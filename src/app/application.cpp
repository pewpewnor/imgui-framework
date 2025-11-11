#include "application.h"

#include "engine/node.h"

class DemoWindow : public engine::Node<SharedState> {
public:
    DemoWindow(const std::shared_ptr<SharedState>& state)
        : engine::Node<SharedState>(state) {}

    void render() override { ImGui::ShowDemoWindow(&state->showDemoWindow); }
};

class ExampleWindow : public engine::Node<SharedState> {
public:
    ExampleWindow(const std::shared_ptr<SharedState>& state)
        : engine::Node<SharedState>(state), demoWindow_(DemoWindow(state)) {}

    void render() override {
        ImGuiIO& imguiIO = ImGui::GetIO();

        if (state->showDemoWindow) {
            demoWindow_.render();
        }

        ImGui::Begin("Hello, world!");
        ImGui::TextUnformatted("This is some useful text.");
        ImGui::Checkbox("Demo Window", &state->showDemoWindow);
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
        glfwGetFramebufferSize(state->glfwWindow.get(), &displayWidth,
                               &displayHeight);
        glViewport(0, 0, displayWidth, displayHeight);
        glClearColor(clear_color_.x * clear_color_.w,
                     clear_color_.y * clear_color_.w,
                     clear_color_.z * clear_color_.w, clear_color_.w);
        glClear(GL_COLOR_BUFFER_BIT);
    }

private:
    DemoWindow demoWindow_;
    ImVec4 clear_color_ = {0.45F, 0.55F, 0.60F, 1.0F};
    int counter_ = 0;
    float slider_value_ = 0.0F;
};

void Application::run() {
    auto state = std::make_shared<SharedState>();
    auto demoWindow = std::make_shared<ExampleWindow>(state);

    engine_.initialize(demoWindow, state, "Example App", 1280, 720, true);
    engine_.run();
}
