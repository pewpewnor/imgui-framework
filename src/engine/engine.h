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
template <typename TState>
    requires std::derived_from<TState, engine::State>
class Engine {
public:
    Engine(const std::shared_ptr<TState>& state) : state_(state) {}

    void run() {
        assert(startupSteps_.size() > 0 &&
               "SFML and ImGui needs to have a startup");
        assert(shutdownSteps_.size() > 0 &&
               "SFML and ImGui needs to have a shutdown");
        startup();
        continouslyRenderFrames();
        shutdown();
    }

    void pushStartupStep(
        const std::shared_ptr<engine::StartupStep<TState>>& step) {
        startupSteps_.push_back(step);
    }

    void pushRenderStep(
        const std::shared_ptr<engine::RenderStep<TState>>& step) {
        renderSteps_.push_back(step);
    }

    void pushShutdownStep(
        const std::shared_ptr<engine::ShutdownStep<TState>>& step) {
        shutdownSteps_.push_back(step);
    }

    void requestStop() { state_->engineStopSignal = true; }

private:
    std::shared_ptr<TState> state_;
    std::vector<std::shared_ptr<engine::StartupStep<TState>>> startupSteps_;
    std::vector<std::shared_ptr<engine::RenderStep<TState>>> renderSteps_;
    std::vector<std::shared_ptr<engine::ShutdownStep<TState>>> shutdownSteps_;
    sf::Clock deltaClock_;

    void startup() {
        for (const auto& step : startupSteps_) {
            step->onStartup();
        }
        startupSteps_.clear();
    }

    void continouslyRenderFrames() {
        while (state_->window->isOpen() && !state_->engineStopSignal) {
            while (const auto event = state_->window->pollEvent()) {
                ImGui::SFML::ProcessEvent(*state_->window, *event);

                if (event->template is<sf::Event::Closed>()) {
                    state_->window->close();
                }
            }

            renderFrame();
        }
        renderSteps_.clear();
    }

    void shutdown() {
        for (const auto& step : shutdownSteps_) {
            step->onShutdown();
        }
        shutdownSteps_.clear();
        state_.reset();
    }

    void renderFrame() {
        ImGui::SFML::Update(*state_->window, deltaClock_.restart());

        for (const auto& step : renderSteps_) {
            step->onRender();
        }

        state_->window->clear();
        ImGui::SFML::Render(*state_->window);
        state_->window->display();
    }
};
}
