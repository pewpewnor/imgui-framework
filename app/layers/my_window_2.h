#pragma once

#include "components/button.h"
#include "engine/render_step.h"
#include "globals/app_state.h"

class MyWindow2 : public engine::RenderStep {
public:
    bool shouldRender() override { return globals::appState->showWindow2; }

    void onRender() override {
        ImGui::Begin("Window2", &globals::appState->showWindow2);
        if (components::customButton("Close")) {
            globals::appState->showWindow2 = false;
        }
        // ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = bg_color_;
        // ImGui::ColorEdit3("Background color", &bg_color_.x);
        ImGui::End();
    }

private:
    ImVec4 bg_color_{0.45F, 0.55F, 0.60F, 1.0F};
};
