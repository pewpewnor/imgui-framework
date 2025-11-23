#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#include <csignal>

#include "application.hpp"

namespace {

void handleStopSignal(int signal) {
    spdlog::info("Signal {} received", signal);
    try {
        Application::requestStop();
    } catch (const std::exception& error) {
        spdlog::error("Error when reacting to signal {}: {}", signal, error.what());
    } catch (...) {
        spdlog::error("Unknown fatal error occurred");
    }
}

}

int main() {
    std::signal(SIGTERM, handleStopSignal);
    std::signal(SIGINT, handleStopSignal);

#ifndef NDEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif

    try {
        spdlog::info("Setting up application...");
        Application app;
        Application::start();
    } catch (const std::exception& error) {
        spdlog::error("Error occurred: {}", error.what());
        return 1;
    } catch (...) {
        spdlog::error("Unknown fatal error occurred");
        return 1;
    }
    spdlog::debug("Main thread finished successfully");
    return 0;
}
