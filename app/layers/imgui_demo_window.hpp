#pragma once

#include <imgui.h>

#include "engine/render_step.hpp"
#include "globals/app_state.hpp"

class ImguiDemoWindow : public engine::RenderStep {
public:
    bool shouldRender() override { return globals::appState->showDemoWindow; }

    void onRender() override { ImGui::ShowDemoWindow(); }
};
