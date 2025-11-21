#include "application.h"

#include <memory>

#include "engine/engine.h"
#include "engine/render_step.h"
#include "globals/app_state.h"
#include "globals/tasks.h"
#include "imgui.h"
#include "spdlog/spdlog.h"
#include "steps/globals_lifetime.h"
#include "steps/surface_lifetime.h"
#include "utils/assertions.h"
#include "utils/key_press_detector.h"
#include "utils/style_counter.h"

namespace components {

bool customButton(const char* label, ImVec2 size = ImVec2(120, 60)) {
    StyleCounter style;
    style.pushStyleColor(ImGuiCol_Button, ImVec4(0.20F, 0.30F, 0.60F, 1.0F));         // normal
    style.pushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.25F, 0.40F, 0.80F, 1.0F));  // hover
    style.pushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.15F, 0.25F, 0.55F, 1.0F));   // active

    style.pushStyleColor(ImGuiCol_Border, ImVec4(1.0F, 1.0F, 1.0F, 0.9F));  // border color
    style.pushStyleVar(ImGuiStyleVar_FrameRounding, 12.0F);                 // round corners
    style.pushStyleVar(ImGuiStyleVar_FrameBorderSize,
                       3.0F);                                        // border thickness
    style.pushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(14, 10));  // internal padding
    return ImGui::Button(label, size);
}

};

class HotkeysHandler : public engine::RenderStep {
public:
    void onRender() override {
#ifndef NDEBUG
        globals::appState->showDemoWindow = KeyPressDetector::combineKeyPressAndKeyHeld(
            f1_, f2_, globals::appState->showDemoWindow);

        if (space_.hasBeenPressed()) {
            spdlog::debug("Space has been pressed");
            /* if (globals::tasks->greetTask.isAvailable()) {
                globals::tasks->greetTask.execute("Alice", globals::appState->frameCount);
            } else {
                spdlog::debug("Ignored request to spawn since worker is busy");
            } */
            if (globals::tasks->greetTask.isBusy()) {
                spdlog::debug("Canceling greet task since it's busy");
                globals::tasks->greetTask.ignore();
            }
            globals::tasks->greetTask.execute("Alice", globals::appState->frameCount);
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

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

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
        ImGui::TextUnformatted(("counter = " + std::to_string(counter_)).c_str());
        globals::appState->frameCount++;
        ImGui::TextUnformatted(
            ("frame count = " + std::to_string(globals::appState->frameCount)).c_str());

        std::string greetings = "Greetings: ";
        if (globals::tasks->greetTask.hasResult()) {
            if (Result<std::string> result = globals::tasks->greetTask.getResult()) {
                greetings += result.value();
            } else {
                spdlog::error("Error from greet task: {}", result.error());
            }
        }
        ImGui::TextUnformatted(greetings.c_str());

        ImGui::TextUnformatted(("Application average " +
                                std::to_string(1000.0F / imguiIO.Framerate) + " ms/frame (" +
                                std::to_string(imguiIO.Framerate) + " FPS)")
                                   .c_str());
        ImGui::End();
    }

private:
    ImVec4 bg_color_{0.45F, 0.55F, 0.60F, 1.0F};
    int counter_ = 0;
    float slider_value_ = 0;
};

class ImguiDemoWindow : public engine::RenderStep {
public:
    bool shouldRender() override { return globals::appState->showDemoWindow; }

    void onRender() override { ImGui::ShowDemoWindow(); }
};

Application::Application() {
    globals::engine = std::make_unique<engine::Engine>();

    auto globalsLifetime = std::make_shared<GlobalsLifetime>();
    globals::engine->pushStartupStep(globalsLifetime);
    globals::engine->pushShutdownStep(globalsLifetime);

    auto surfaceLifetime = std::make_shared<SurfaceLifetime>("Example App", 1280, 720);
    globals::engine->pushStartupStep(surfaceLifetime);
    globals::engine->pushShutdownStep(surfaceLifetime);

    globals::engine->pushRenderStep(std::make_shared<HotkeysHandler>());
    globals::engine->pushRenderStep(std::make_shared<MyDemoWindow>());
    globals::engine->pushRenderStep(std::make_shared<ImguiDemoWindow>());
}

Application::~Application() { globals::engine.reset(); }

void Application::start() {
    ASSERT(globals::engine, "only execute application with engine existing");
    spdlog::info("Running application...");
    globals::engine->runContinously();
    spdlog::info("Application stopped");
}

void Application::requestStop() {
    if (globals::engine) {
        spdlog::debug("Sending stop signal to engine...");
        globals::engine->sendStopSignal();
    }
}
