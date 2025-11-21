#include "engine.h"

#include <SFML/System.hpp>
#include <mutex>
#include <ranges>
#include <stdexcept>

#include "engine/render_step.h"
#include "engine/shutdown_step.h"
#include "engine/startup_step.h"
#include "utils/assertions.h"

namespace {

constexpr sf::Time fpsToTimePerFrame(int fps) { return sf::milliseconds(1000 / fps); }

}

void engine::Engine::runContinously() {
    bool expected = false;
    if (!isRunning_.compare_exchange_strong(expected, true)) {
        throw std::runtime_error("engine is already running");
    }
    try {
        startup();
        renderFramesContinously();
    } catch (...) {
        shutdown();
        throw;
    }
    shutdown();
}

void engine::Engine::pushStartupStep(const std::shared_ptr<engine::StartupStep>& step) {
    ASSERT(!isRunning_, "only add step while engine is not running");
    startupSteps_.push_back(step);
}

void engine::Engine::pushRenderStep(const std::shared_ptr<engine::RenderStep>& step) {
    ASSERT(!isRunning_, "only add step while engine is not running");
    renderSteps_.push_back(step);
}

void engine::Engine::pushShutdownStep(const std::shared_ptr<engine::ShutdownStep>& step) {
    ASSERT(!isRunning_, "only add step while engine is not running");
    shutdownSteps_.push_back(step);
}

void engine::Engine::sendStopSignal() { stopSignal_ = true; }

void engine::Engine::sendRefreshSignal() { refreshSignal_ = true; }

void engine::Engine::waitUntilStopped() {
    std::unique_lock<std::mutex> lock(runningMutex_);
    runningCv_.wait(lock, [this] { return !isRunning_; });
}

void engine::Engine::startup() {
    refreshSignal_ = true;
    for (const std::shared_ptr<engine::StartupStep>& startupStep : startupSteps_) {
        startupStep->onStartup();
    }
}

void engine::Engine::renderFramesContinously() {
    sf::Clock clock;
    while (!stopSignal_ && window->isOpen()) {
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
    bool hasFocus = window->hasFocus();
    bool refresh = false;
    while (const auto event = window->pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            sendStopSignal();
            break;
        }
        if (event->is<sf::Event::FocusLost>()) {
            refresh = true;
            continue;
        }
        ImGui::SFML::ProcessEvent(*window, *event);
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
    if (refreshSignal_.compare_exchange_strong(expected, false)) {
        refresh = true;
    }
    return refresh;
}

void engine::Engine::renderFrame() {
    ImGui::SFML::Update(*window, deltaClock_.restart());

    for (const std::shared_ptr<engine::RenderStep>& renderStep : renderSteps_) {
        if (renderStep->shouldRender()) {
            renderStep->onRender();
        }
    }

    window->clear();
    ImGui::SFML::Render(*window);
    window->display();
}

void engine::Engine::shutdown() {
    try {
        for (const std::shared_ptr<engine::ShutdownStep>& shutdownStep :
             std::ranges::reverse_view(shutdownSteps_)) {
            shutdownStep->onShutdown();
        }
    } catch (...) {
        stopRunningState();
        throw;
    }
    stopRunningState();
}

void engine::Engine::stopRunningState() {
    imguiInitialized_ = false;
    stopSignal_ = false;
    std::lock_guard<std::mutex> lock(runningMutex_);
    isRunning_ = false;
    runningCv_.notify_all();
}
