#include "application.h"

#include "engine/glfw_imgui_surface.h"

class RenderDemo : public engine::RenderStep<SharedState> {
public:
    RenderDemo(const std::shared_ptr<SharedState>& state)
        : engine::Step<SharedState>(state),
          clear_color_{0.45F, 0.55F, 0.60F, 1.0F},
          counter_(0),
          slider_value_(0.0F) {}

    void onRender() override {
        ImGuiIO& imguiIO = ImGui::GetIO();

        if (state->showDemoWindow) {
            ImGui::ShowDemoWindow(&state->showDemoWindow);
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
    ImVec4 clear_color_;
    int counter_;
    float slider_value_;
};

void Application::run() {
    auto state = std::make_shared<SharedState>();

    engine_ = std::make_unique<engine::Engine<SharedState>>(state);

    auto glfwImguiSurface =
        std::make_shared<engine::GlfwImguiSurface<SharedState>>(
            state, "Example App", 1280, 720, true);
    engine_->addStartupStep(glfwImguiSurface);
    engine_->addShutdownStep(glfwImguiSurface);

    engine_->addRenderStep(std::make_shared<RenderDemo>(state));

    engine_->run();
}
