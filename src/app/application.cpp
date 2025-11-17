#include "application.h"

#include <chrono>
#include <cmath>
#include <iostream>
#include <thread>

#include "SFML/Window/Keyboard.hpp"
#include "engine/engine.h"
#include "engine/engine_state.h"
#include "engine/render_step.h"
#include "engine/surface.h"
#include "globals.h"
#include "imgui.h"
#include "key_press_detector.h"
#include "style_stack.h"

namespace components {

bool customButton(const char* label, ImVec2 size = ImVec2(120, 60)) {
    StyleStack style;
    style.pushStyleColor(ImGuiCol_Button,
                         ImVec4(0.20F, 0.30F, 0.60F, 1.0F));  // normal
    style.pushStyleColor(ImGuiCol_ButtonHovered,
                         ImVec4(0.25F, 0.40F, 0.80F, 1.0F));  // hover
    style.pushStyleColor(ImGuiCol_ButtonActive,
                         ImVec4(0.15F, 0.25F, 0.55F, 1.0F));  // active

    style.pushStyleColor(ImGuiCol_Border,
                         ImVec4(1.0F, 1.0F, 1.0F, 0.9F));    // border color
    style.pushStyleVar(ImGuiStyleVar_FrameRounding, 12.0F);  // round corners
    style.pushStyleVar(ImGuiStyleVar_FrameBorderSize,
                       3.0F);  // border thickness
    style.pushStyleVar(ImGuiStyleVar_FramePadding,
                       ImVec2(14, 10));  // internal padding
    return ImGui::Button(label, size);
}

};

class HotkeysHandler : public engine::RenderStep {
public:
    void onRender() override {
#ifndef NDEBUG
        globals::appState->showDemoWindow =
            KeyPressDetector::combineKeyPressAndKeyHeld(
                f1_, f2_, globals::appState->showDemoWindow);

        if (space_.hasBeenPressed()) {
            std::string name = "Alice";
            globals::appState->sleepWorker.spawn([name]() {
                std::this_thread::sleep_for(std::chrono::seconds(2));
                globals::appState->greetings = "Greetings from var: " + name;
                globals::engineState->refreshSignal = true;
                return "Greetings from result: " + name;
            });
            std::cout << "space pressed" << std::endl;
        }
#endif
    }

private:
    KeyPressDetector f1_{sf::Keyboard::Key::F1};
    KeyPressDetector f2_{sf::Keyboard::Key::F2};
    KeyPressDetector space_{sf::Keyboard::Key::Space};
};

class MyDemoWindow : public engine::RenderStep {
public:
    void onRender() override {
        ImGuiIO& imguiIO = ImGui::GetIO();

        auto flags = static_cast<ImGuiWindowFlags>(
            static_cast<unsigned int>(ImGuiWindowFlags_NoDecoration) |
            static_cast<unsigned int>(ImGuiWindowFlags_NoResize) |
            static_cast<unsigned int>(ImGuiWindowFlags_NoMove) |
            static_cast<unsigned int>(ImGuiWindowFlags_NoBringToFrontOnFocus));

        // --- 2. Set Position and Size to Match Viewport ---
        // The ImGui backend (SFML/GLFW) is responsible for clearing the window
        // background color. We make the ImGui window cover the viewport
        // exactly.
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        // --- 3. BEGIN THE FULL-SCREEN WINDOW ---
        // Pass a unique ID (e.g., "MainAppCanvas") and the flags.
        ImGui::Begin("MainAppCanvas", nullptr, flags);

        // ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = bg_color_;

        ImGui::TextUnformatted("This is some useful text.");
        ImGui::Checkbox("Demo Window", &globals::appState->showDemoWindow);
        ImGui::SliderFloat("float", &slider_value_, 0.0F, 1.0F);
        // ImGui::ColorEdit3("Background color", &bg_color_.x);

        if (components::customButton("Custom Button")) {
            counter_++;
        }
        ImGui::SameLine();
        ImGui::TextUnformatted(
            ("counter = " + std::to_string(counter_)).c_str());
        ImGui::TextUnformatted(
            ("infinite = " + std::to_string(infinite_++)).c_str());
        if (globals::appState->sleepWorker.resultIsReady()) {
            ImGui::TextUnformatted(
                globals::appState->sleepWorker.getResultBlocking()->c_str());
        }
        ImGui::TextUnformatted(
            ("The greetings: " + globals::appState->greetings).c_str());

        ImGui::TextUnformatted(("Application average " +
                                std::to_string(1000.0F / imguiIO.Framerate) +
                                " ms/frame (" +
                                std::to_string(imguiIO.Framerate) + " FPS)")
                                   .c_str());
        ImGui::End();
    }

private:
    ImVec4 bg_color_{0.45F, 0.55F, 0.60F, 1.0F};
    int counter_ = 0;
    float slider_value_ = 0;
    int infinite_ = 0;
};

class ImguiDemoWindow : public engine::RenderStep {
public:
    bool shouldRender() override { return globals::appState->showDemoWindow; }

    void onRender() override { ImGui::ShowDemoWindow(); }
};

Application::Application() {
    globals::appState = std::make_shared<AppState>();
    globals::engineState = std::make_shared<engine::EngineState>();
    engine_ = std::make_unique<engine::Engine>(globals::engineState);

    auto surface = std::make_shared<engine::Surface>(globals::engineState,
                                                     "Example App", 1280, 720);
    engine_->pushStartupStep(surface);
    engine_->pushShutdownStep(surface);

    engine_->pushRenderStep(std::make_shared<HotkeysHandler>());
    engine_->pushRenderStep(std::make_shared<MyDemoWindow>());
    engine_->pushRenderStep(std::make_shared<ImguiDemoWindow>());
}

void Application::execute() { engine_->runContinously(); }

Application::~Application() {
    globals::appState.reset();
    globals::engineState.reset();
}
