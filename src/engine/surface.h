#pragma once

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>

#include "shutdown_step.h"
#include "startup_step.h"

namespace engine {

class Surface : public engine::StartupStep, public engine::ShutdownStep {
public:
    Surface(const std::string& title, int width, int height, bool vsync);

    void onStartup() override;

    void onShutdown() override;

private:
    std::string title_;
    int width_ = 1280;
    int height_ = 720;
    bool vsync_ = true;
};
}
