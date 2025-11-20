#include "application.h"

#include <chrono>
#include <iostream>
#include <memory>
#include <thread>

#include "engine/engine.h"
#include "engine/render_step.h"
#include "globals.h"
#include "imgui.h"
#include "spdlog/spdlog.h"
#include "utils/assertion.h"
#include "utils/key_press_detector.h"
#include "utils/result.h"
#include "utils/style_stack.h"

namespace components {

bool customButton(const char* label, ImVec2 size = ImVec2(120, 60)) {
    StyleStack style;
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
            if (globals::workers->sleepWorker.isFreeToSpawn()) {
                std::string name = "Alice";
                // TODO: make the task into a class instead, the optional inside of the task class
                // instead of on the async worker
                // it should be future<void>
                // the task class should extend a cache class which caches the result and shit
                globals::workers->sleepWorker.spawnBlocking(
                    [name, frameCount = globals::appState->frameCount]() {
                        // spdlog::debug("Spawned sleep worker");
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                        return "Hello, " + name + " " + std::to_string(frameCount) + "!";
                    },
                    []() {
                        // spdlog::debug("Sent refresh signal...");
                        globals::engine->sendRefreshSignal();
                    });
            } else {
                std::cout << "ignored request since already working" << std::endl;
            }
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
        if (globals::workers->sleepWorker.hasResult()) {
            if (Result<std::string> result = globals::workers->sleepWorker.getResultBlocking()) {
                greetings += result.value();
            } else {
                std::cout << "Error: " + result.error() << std::endl;
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
    globals::engine = std::make_shared<engine::Engine>("Example App", 1280, 720);
    globals::appState = std::make_shared<AppState>();
    globals::workers = std::make_shared<Workers>();

    globals::engine->pushRenderStep(std::make_shared<HotkeysHandler>());
    globals::engine->pushRenderStep(std::make_shared<MyDemoWindow>());
    globals::engine->pushRenderStep(std::make_shared<ImguiDemoWindow>());
}

Application::~Application() {
    spdlog::info("Stopping application...");
    globals::workers.reset();
    globals::appState.reset();
    globals::engine.reset();
    spdlog::info("Application stopped");
}

void Application::start() {
    ASSERT_SOFT(globals::engine, "application executed with engine existing");
    if (globals::engine) {
        globals::engine->runContinously();
    }
}

void Application::stop() {
    ASSERT_SOFT(globals::engine, "application executed with engine existing");
    if (globals::engine) {
        spdlog::debug("Sending stop signal to engine...");
        globals::engine->sendStopSignal();
    }
}
