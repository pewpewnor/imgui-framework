#pragma once

#include <imgui.h>

#include <string>

#include "engine/render_step.hpp"
#include "globals/dispatch_state.hpp"
#include "luaren/lua_renderer.hpp"

class MainWindow : public engine::RenderStep {
public:
    void onRender() override {
        luaRenderer_.registerGlobalBool("showImguiDemoWindow", &g::dispatch->showImguiDemoWindow);
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);

        auto windowFlags = static_cast<ImGuiWindowFlags>(
            static_cast<unsigned int>(ImGuiWindowFlags_NoDecoration) |
            static_cast<unsigned int>(ImGuiWindowFlags_NoResize) |
            static_cast<unsigned int>(ImGuiWindowFlags_NoMove) |
            static_cast<unsigned int>(ImGuiWindowFlags_NoBringToFrontOnFocus));
        ImGui::Begin("MainWindow", nullptr, windowFlags);

        if (g::dispatch->showImguiDemoWindow) {
            ImGui::ShowDemoWindow(&g::dispatch->showImguiDemoWindow);
        }

        luaRenderer_.render();

        ImVec2 displaySize = ImGui::GetIO().DisplaySize;
        ImGui::SetNextWindowPos(ImVec2(displaySize.x, 0), ImGuiCond_Always, ImVec2(1, 0));
        /* auto windowFlags2 = static_cast<ImGuiWindowFlags>(
            static_cast<unsigned int>(ImGuiWindowFlags_NoDecoration) |
            static_cast<unsigned int>(ImGuiWindowFlags_NoBackground) |
            static_cast<unsigned int>(ImGuiWindowFlags_NoResize) |
            static_cast<unsigned int>(ImGuiWindowFlags_NoMove) |
            static_cast<unsigned int>(ImGuiWindowFlags_NoBringToFrontOnFocus)); */
        ImGui::Begin("Overlay", nullptr);
        ImGui::TextUnformatted(
            ("Frame count = " + std::to_string(frameCount++) +
             " ; Application average = " + std::to_string(1000.0F / ImGui::GetIO().Framerate) +
             " ms/frame (" + std::to_string(ImGui::GetIO().Framerate) + " FPS)")
                .c_str());
        ImGui::End();

        ImGui::End();
    }

private:
    static inline unsigned int frameCount = 1;

    LuaRenderer luaRenderer_{"/home/braum/Burn/Git/imgui-example/lua/script.lua"};
};
