#include "application.h"

#include <cmath>

#include "SFML/Window/Keyboard.hpp"
#include "app/globals.h"
#include "app/key_press_detector.h"
#include "engine/surface.h"

bool customButton(const char* label, ImVec2 size = ImVec2(200, 60)) {
    ImGui::PushStyleColor(ImGuiCol_Button,
                          ImVec4(0.20F, 0.30F, 0.60F, 1.0F));  // normal
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4(0.25F, 0.40F, 0.80F, 1.0F));  // hover
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          ImVec4(0.15F, 0.25F, 0.55F, 1.0F));  // active

    ImGui::PushStyleColor(ImGuiCol_Border,
                          ImVec4(1.0F, 1.0F, 1.0F, 0.9F));    // border color
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 12.0F);  // round corners
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize,
                        3.0F);  // border thickness
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
                        ImVec2(14, 10));  // internal padding

    bool pressed = ImGui::Button(label, size);

    ImGui::PopStyleVar(3);
    ImGui::PopStyleColor(4);

    return pressed;
}

class RenderDemo : public engine::RenderStep {
public:
    void onRender() override {
        ImGuiIO& imguiIO = ImGui::GetIO();

        ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = bg_color_;

#ifndef NDEBUG
        appState.showDemoWindow = KeyPressDetector::combineKeyPressAndKeyHeld(
            f1_, f2_, appState.showDemoWindow);
#endif

        if (appState.showDemoWindow) {
            ImGui::ShowDemoWindow(&appState.showDemoWindow);
        }

        ImGui::Begin("Hello, world!");
        ImGui::TextUnformatted("This is some useful text.");
        ImGui::Checkbox("Demo Window", &appState.showDemoWindow);
        ImGui::SliderFloat("float", &slider_value_, 0.0F, 1.0F);
        ImGui::ColorEdit3("Background color", &bg_color_.x);

        if (customButton("Button")) {
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
    }

private:
    KeyPressDetector f1_ = KeyPressDetector(sf::Keyboard::Key::F1);
    KeyPressDetector f2_ = KeyPressDetector(sf::Keyboard::Key::F2);
    ImVec4 bg_color_ = {0.45F, 0.55F, 0.60F, 1.0F};
    int counter_ = 0;
    float slider_value_ = 0;
};

void Application::run() {
    auto surface =
        std::make_shared<engine::Surface>("Example App", 1280, 720, true);
    engine_.pushStartupStep(surface);
    engine_.pushShutdownStep(surface);

    engine_.pushRenderStep(std::make_shared<RenderDemo>());

    engine_.run();
}
