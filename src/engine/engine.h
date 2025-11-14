#pragma once

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <vector>

#include "render_step.h"
#include "shutdown_step.h"
#include "startup_step.h"

namespace engine {

class Engine {
public:
    void run();

    void pushStartupStep(const std::shared_ptr<engine::StartupStep>& step);

    void pushRenderStep(const std::shared_ptr<engine::RenderStep>& step);

    void pushShutdownStep(const std::shared_ptr<engine::ShutdownStep>& step);

private:
    std::vector<std::shared_ptr<engine::StartupStep>> startupSteps_;
    std::vector<std::shared_ptr<engine::RenderStep>> renderSteps_;
    std::vector<std::shared_ptr<engine::ShutdownStep>> shutdownSteps_;
    sf::Clock deltaClock_;

    void startup();

    void continouslyRenderFrames();

    void shutdown();

    void renderFrame();
};
}
