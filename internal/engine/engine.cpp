#include "engine.h"

#include <SFML/System.hpp>
#include <exception>
#include <stdexcept>

#include "utils/assertions.h"

namespace {

constexpr sf::Time fpsToTimePerFrame(int fps) { return sf::milliseconds(1000 / fps); }

}

engine::Engine::Engine(const std::string& title, int width, int height)
    : window_(std::make_shared<sf::RenderWindow>(
          sf::VideoMode({static_cast<unsigned int>(width), static_cast<unsigned int>(height)}),
          title)) {}

void engine::Engine::runContinously() {
    bool expected = false;
    bool desired = true;
    if (!isRunning_.compare_exchange_strong(expected, desired)) {
        throw std::runtime_error("engine is already running");
    }
    try {
        startup();
        renderFramesContinously();
        shutdown();
    } catch (const std::exception& error) {
        isRunning_ = false;
        throw error;
    }
}

void engine::Engine::pushStartupStep(const std::shared_ptr<engine::StartupStep>& step) {
    ASSERT(!isRunning_, "cannot add startup step while engine is running");
    startupSteps_.push_back(step);
}

void engine::Engine::pushRenderStep(const std::shared_ptr<engine::RenderStep>& step) {
    ASSERT(!isRunning_, "cannot add render step while engine is running");
    renderSteps_.push_back(step);
}

void engine::Engine::pushShutdownStep(const std::shared_ptr<engine::ShutdownStep>& step) {
    ASSERT(!isRunning_, "cannot add shutdown step while engine is running");
    shutdownSteps_.push_back(step);
}

void engine::Engine::sendStopSignal() { stopSignal_ = true; }

void engine::Engine::sendRefreshSignal() { refreshSignal_ = true; }

void engine::Engine::startup() {
    stopSignal_ = false;
    refreshSignal_ = false;

    window_->setVerticalSyncEnabled(true);
    if (!ImGui::SFML::Init(*window_)) {
        throw std::runtime_error("failed to initialize imgui-sfml");
    }
    ImGui::StyleColorsDark();

    for (const auto& step : startupSteps_) {
        step->onStartup();
    }
}

void engine::Engine::renderFramesContinously() {
    sf::Clock clock;
    while (window_->isOpen() && !stopSignal_) {
        clock.restart();

        bool refresh = processEvents();

        sf::Time elapsed;
        sf::Time desiredDuration;
        if (refresh) {
            renderFrame();
            elapsed = clock.getElapsedTime();
            desiredDuration = fpsToTimePerFrame(70);
        } else {
            elapsed = clock.getElapsedTime();
            desiredDuration = fpsToTimePerFrame(30);
        }
        if (sf::Time sleepTime = desiredDuration - elapsed; sleepTime > sf::Time::Zero) {
            sf::sleep(sleepTime);
        }
    }
}

bool engine::Engine::processEvents() {
    bool hasFocus = window_->hasFocus();
    bool refresh = false;
    while (const auto event = window_->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            sendStopSignal();
            break;
        }
        if (event->is<sf::Event::FocusLost>()) {
            refresh = true;
            continue;
        }
        ImGui::SFML::ProcessEvent(*window_, *event);
        if (!refresh &&
            (hasFocus || event->is<sf::Event::FocusGained>() || event->is<sf::Event::Resized>() ||
             event->is<sf::Event::MouseButtonPressed>() || event->is<sf::Event::MouseEntered>() ||
             event->is<sf::Event::MouseLeft>() || event->is<sf::Event::MouseMoved>() ||
             event->is<sf::Event::MouseWheelScrolled>())) {
            refresh = true;
        }
    }
    if (!refresh && hasFocus && ImGui::GetIO().WantTextInput) {
        refresh = true;
    }
    bool expected = true;
    bool desired = false;
    if (refreshSignal_.compare_exchange_strong(expected, desired)) {
        refresh = true;
    }
    return refresh;
}

void engine::Engine::renderFrame() {
    ImGui::SFML::Update(*window_, deltaClock_.restart());

    for (const auto& step : renderSteps_) {
        if (step->shouldRender()) {
            step->onRender();
        }
    }

    window_->clear();
    ImGui::SFML::Render(*window_);
    window_->display();
}

void engine::Engine::shutdown() {
    for (const auto& step : shutdownSteps_) {
        step->onShutdown();
    }

    window_->close();
    ImGui::SFML::Shutdown();
    isRunning_ = false;
}
