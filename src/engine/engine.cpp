#include "engine.h"

#include <SFML/System.hpp>

void engine::Engine::initialize(
    const std::shared_ptr<engine::EngineState>& engineState) {
    engineState_ = engineState;
}

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
    for (const auto& step : startupSteps_) {
        step->onStartup();
    }
    startupSteps_.clear();
}

void engine::Engine::renderFramesContinously() {
    sf::Time idleSleepTime = sf::milliseconds(16);

    while (engineState_->window.isOpen() && !engineState_->stopSignal) {
        bool refresh = false;

        while (const auto event = engineState_->window.pollEvent()) {
            ImGui::SFML::ProcessEvent(engineState_->window, *event);
            if (event->template is<sf::Event::Closed>()) {
                engineState_->window.close();
            }
            refresh = true;
        }
        if (ImGui::GetIO().WantTextInput) {
            refresh = true;
        }
        if (engineState_->refreshSignal) {
            refresh = true;
            engineState_->refreshSignal = false;
        }

        if (refresh) {
            renderFrame();
        } else {
            sf::sleep(idleSleepTime);
        }
    }
    renderSteps_.clear();
}

void engine::Engine::shutdown() {
    for (const auto& step : shutdownSteps_) {
        step->onShutdown();
    }
    shutdownSteps_.clear();
    engineState_.reset();
}

void engine::Engine::renderFrame() {
    ImGui::SFML::Update(engineState_->window, deltaClock_.restart());

    for (const auto& step : renderSteps_) {
        if (step->shouldRender()) {
            step->onRender();
        }
    }

    engineState_->window.clear(sf::Color::White);
    ImGui::SFML::Render(engineState_->window);
    engineState_->window.display();
}
