#include "surface.h"

#include "engine_state.h"
#include "imgui.h"

engine::Surface::Surface(const std::string& title, int width, int height,
                         bool vsync)
    : title_(title), width_(width), height_(height), vsync_(vsync) {}

void engine::Surface::onStartup() {
    engineState.window =
        sf::RenderWindow(sf::VideoMode({static_cast<unsigned int>(width_),
                                        static_cast<unsigned int>(height_)}),
                         title_);
    if (vsync_) {
        engineState.window.setVerticalSyncEnabled(true);
    } else {
        engineState.window.setFramerateLimit(60);
    }
    if (!ImGui::SFML::Init(engineState.window)) {
        throw std::runtime_error("failed to initialize ImGui-SFML");
    }
    ImGui::StyleColorsDark();
}

void engine::Surface::onShutdown() { ImGui::SFML::Shutdown(); }
