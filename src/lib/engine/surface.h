#pragma once

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "engine_state.h"
#include "shutdown_step.h"
#include "startup_step.h"

namespace engine {

class Surface : public engine::StartupStep, public engine::ShutdownStep {
public:
    Surface(const std::shared_ptr<engine::EngineState>& engineState,
            const std::string& title, int width, int height);

    void onStartup() override;

    void onShutdown() override;

private:
    std::shared_ptr<engine::EngineState> engineState_;
    std::string title_;
    int width_;
    int height_;
};
}
