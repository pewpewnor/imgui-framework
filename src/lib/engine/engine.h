#pragma once

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

#include "engine_state.h"
#include "render_step.h"
#include "shutdown_step.h"
#include "startup_step.h"

namespace engine {

class Engine {
public:
    Engine(const std::shared_ptr<engine::EngineState>& state);

    void runContinously();

    void pushStartupStep(const std::shared_ptr<engine::StartupStep>& step);

    void pushRenderStep(const std::shared_ptr<engine::RenderStep>& step);

    void pushShutdownStep(const std::shared_ptr<engine::ShutdownStep>& step);

private:
    std::shared_ptr<engine::EngineState> state_;
    std::vector<std::shared_ptr<engine::StartupStep>> startupSteps_;
    std::vector<std::shared_ptr<engine::RenderStep>> renderSteps_;
    std::vector<std::shared_ptr<engine::ShutdownStep>> shutdownSteps_;
    sf::Clock deltaClock_;

    void startup();

    void renderFramesContinously();

    void shutdown();

    void renderFrame();
};
}
