#pragma once

#include "components/button.h"
#include "engine/render_step.h"
#include "globals/app_state.h"
#include "globals/tasks.h"
#include "imgui.h"
#include "spdlog/spdlog.h"
#include "utils/results.h"

class MyWindow : public engine::RenderStep {
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

        ImGui::TextUnformatted("This is some useful text.");
        ImGui::Checkbox("Demo Window##checkbox", &globals::appState->showDemoWindow);
        ImGui::SliderFloat("float", &slider_value_, 0.0F, 1.0F);

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

        if (components::customButton("Demo Window##button")) {
            globals::appState->showDemoWindow = !globals::appState->showDemoWindow;
        }
        if (components::customButton("Window 2")) {
            globals::appState->showWindow2 = true;
        }

        ImGui::TextUnformatted(("Application average " +
                                std::to_string(1000.0F / imguiIO.Framerate) + " ms/frame (" +
                                std::to_string(imguiIO.Framerate) + " FPS)")
                                   .c_str());

        ImGui::End();
    }

private:
    int counter_ = 0;
    float slider_value_ = 0;
};
