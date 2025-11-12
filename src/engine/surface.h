#pragma once

#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window/VideoMode.hpp>
#include <memory>

#include "shutdown_step.h"
#include "startup_step.h"
#include "step.h"

namespace engine {
template <typename TState>
    requires std::derived_from<TState, engine::State>
class Surface : public engine::StartupStep<TState>,
                public engine::ShutdownStep<TState> {
public:
    Surface(const std::shared_ptr<TState>& state, const std::string& title,
            int width, int height, bool vsync)
        : engine::Step<TState>(state),
          title_(title),
          width_(width),
          height_(height),
          vsync_(vsync) {}

    void onStartup() override {
        initializeSfml();
        initializeImgui();
    }

    void onShutdown() override {
        ImGui::SFML::Shutdown();
        this->state->window.reset();
    }

private:
    std::string title_;
    int width_ = 1280;
    int height_ = 720;
    bool vsync_ = true;

    void initializeSfml() {
        this->state->window = std::make_shared<sf::RenderWindow>(
            sf::VideoMode({static_cast<unsigned int>(width_),
                           static_cast<unsigned int>(height_)}),
            title_);

        if (!this->state->window) {
            throw std::runtime_error("failed to create SFML window");
        }

        if (vsync_) {
            this->state->window->setVerticalSyncEnabled(true);
        } else {
            this->state->window->setFramerateLimit(60);
        }
    }

    void initializeImgui() const {
        if (!ImGui::SFML::Init(*this->state->window)) {
            throw std::runtime_error("failed to initialize ImGui-SFML");
        }
        ImGui::StyleColorsDark();
    }
};
}
