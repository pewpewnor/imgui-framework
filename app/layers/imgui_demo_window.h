#pragma once

#include "engine/render_step.h"
#include "globals/app_state.h"
#include "imgui.h"

class ImguiDemoWindow : public engine::RenderStep {
public:
    bool shouldRender() override { return globals::appState->showDemoWindow; }

    void onRender() override { ImGui::ShowDemoWindow(); }
};
