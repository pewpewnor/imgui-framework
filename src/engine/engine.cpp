#include "engine.h"

#include "engine_state.h"

void engine::Engine::run() {
    assert(startupSteps_.size() > 0 &&
           "SFML and ImGui needs to have a startup");
    assert(shutdownSteps_.size() > 0 &&
           "SFML and ImGui needs to have a shutdown");
    startup();
    continouslyRenderFrames();
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

void engine::Engine::continouslyRenderFrames() {
    while (engineState.window.isOpen() && !engineState.stopSignal) {
        while (const auto event = engineState.window.pollEvent()) {
            ImGui::SFML::ProcessEvent(engineState.window, *event);

            if (event->template is<sf::Event::Closed>()) {
                engineState.window.close();
            }
        }
        renderFrame();
    }
    renderSteps_.clear();
}

void engine::Engine::shutdown() {
    for (const auto& step : shutdownSteps_) {
        step->onShutdown();
    }
    shutdownSteps_.clear();
    engineState = EngineState();
}

void engine::Engine::renderFrame() {
    ImGui::SFML::Update(engineState.window, deltaClock_.restart());

    for (const auto& step : renderSteps_) {
        step->onRender();
    }

    engineState.window.clear(sf::Color::White);
    ImGui::SFML::Render(engineState.window);
    engineState.window.display();
}
