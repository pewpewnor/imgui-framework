#include "engine.h"

#include <SFML/System.hpp>
#include <mutex>
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
    ASSERT(!isRunning_, "running engine that is stopped");
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
    ASSERT(!isRunning_, "adding step while engine is not running");
    startupSteps_.push_back(step);
}

void engine::Engine::pushRenderStep(const std::shared_ptr<engine::RenderStep>& step) {
    ASSERT(!isRunning_, "adding step while engine is not running");
    renderSteps_.push_back(step);
}

void engine::Engine::pushShutdownStep(const std::shared_ptr<engine::ShutdownStep>& step) {
    ASSERT(!isRunning_, "adding step while engine is not running");
    shutdownSteps_.push_back(step);
}

void engine::Engine::sendStopSignal() { stopSignal_ = true; }

void engine::Engine::sendRefreshSignal() { refreshSignal_ = true; }

void engine::Engine::waitUntilStopped() {
    std::unique_lock<std::mutex> lock(runningMutex_);
    runningCv_.wait(lock, [this] { return !isRunning_; });
}

void engine::Engine::startup() {
    stopSignal_ = false;
    refreshSignal_ = false;

    window_->setVerticalSyncEnabled(true);
    if (!ImGui::SFML::Init(*window_)) {
        throw std::runtime_error("failed to initialize imgui-sfml");
    }
    imguiInitialized_ = true;
    ImGui::StyleColorsDark();

    for (const auto& step : startupSteps_) {
        step->onStartup();
    }
}

void engine::Engine::renderFramesContinously() {
    sf::Clock clock;
    while (!stopSignal_ && window_->isOpen()) {
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
    if (refreshSignal_.compare_exchange_strong(expected, false)) {
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
    try {
        for (const auto& step : shutdownSteps_) {
            step->onShutdown();
        }
        window_->close();
        if (imguiInitialized_) {
            ImGui::SFML::Shutdown();
            imguiInitialized_ = false;
        }
    } catch (...) {
        imguiInitialized_ = false;
        setRunningState(false);
        throw;
    }
    imguiInitialized_ = false;
    setRunningState(false);
}

void engine::Engine::setRunningState(bool isRunning) {
    std::lock_guard<std::mutex> lock(runningMutex_);
    isRunning_ = isRunning;
    runningCv_.notify_all();
}
