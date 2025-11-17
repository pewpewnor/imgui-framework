#include "engine.h"

#include <SFML/System.hpp>

#include "SFML/System/Time.hpp"
#include "engine_state.h"

namespace {
constexpr sf::Time timePerFrameMillisecFromFps(int fps) {
    return sf::milliseconds(1000 / fps);
}
}

engine::Engine::Engine(const std::shared_ptr<engine::EngineState>& state)
    : state_(state) {}

void engine::Engine::runContinously() {
    assert(startupSteps_.size() > 0 &&
           "SFML and ImGui needs to have a startup");
    assert(shutdownSteps_.size() > 0 &&
           "SFML and ImGui needs to have a shutdown");
    startup();
    renderFramesContinously();
    shutdown();
}

void engine::Engine::pushStartupStep(
    const std::shared_ptr<engine::StartupStep>& step) {
    startupSteps_.push_back(step);
}

void engine::Engine::pushRenderStep(
    const std::shared_ptr<engine::RenderStep>& step) {
    renderSteps_.push_back(step);
}

void engine::Engine::pushShutdownStep(
    const std::shared_ptr<engine::ShutdownStep>& step) {
    shutdownSteps_.push_back(step);
}

void engine::Engine::startup() {
    state_->stopSignal = false;
    state_->refreshSignal = false;
    for (const auto& step : startupSteps_) {
        step->onStartup();
    }
}

void engine::Engine::renderFramesContinously() {
    sf::Time idleSleepTime = timePerFrameMillisecFromFps(20);

    while (state_->window.isOpen() && !state_->stopSignal) {
        bool hasFocus = state_->window.hasFocus();
        bool refresh = false;

        while (const auto event = state_->window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                return;
            }
            if (event->is<sf::Event::FocusLost>()) {
                continue;
            }
            ImGui::SFML::ProcessEvent(state_->window, *event);

            if (hasFocus || event->is<sf::Event::FocusGained>() ||
                event->is<sf::Event::Resized>() ||
                event->is<sf::Event::MouseButtonPressed>() ||
                event->is<sf::Event::MouseEntered>() ||
                event->is<sf::Event::MouseLeft>() ||
                event->is<sf::Event::MouseMoved>() ||
                event->is<sf::Event::MouseWheelScrolled>()) {
                refresh = true;
            }
        }

        if (hasFocus && ImGui::GetIO().WantTextInput) {
            refresh = true;
        }

        if (state_->refreshSignal) {
            refresh = true;
            state_->refreshSignal = false;
        }

        if (refresh) {
            renderFrame();
        } else {
            sf::sleep(idleSleepTime);
        }
    }
}

void engine::Engine::shutdown() {
    state_->window.close();
    for (const auto& step : shutdownSteps_) {
        step->onShutdown();
    }
    state_->stopSignal = false;
    state_->refreshSignal = false;
}

void engine::Engine::renderFrame() {
    ImGui::SFML::Update(state_->window, deltaClock_.restart());

    for (const auto& step : renderSteps_) {
        if (step->shouldRender()) {
            step->onRender();
        }
    }

    state_->window.clear(sf::Color::White);
    ImGui::SFML::Render(state_->window);
    state_->window.display();
}
