#include <csignal>

#include "application.h"
#include "spdlog/common.h"
#include "spdlog/spdlog.h"

namespace {

void handleStopSignal(int signal) {
    spdlog::info("Signal {} received", signal);
    try {
        Application::stop();
    } catch (const std::exception& error) {
        spdlog::error("Error occurred: {}", error.what());
    } catch (...) {
        spdlog::error("Unknown fatal error occurred");
    }
}

}

int main() {
#ifndef NDEBUG
    spdlog::set_level(spdlog::level::debug);
#else
    spdlog::set_level(spdlog::level::info);
#endif

    std::signal(SIGINT, handleStopSignal);
    std::signal(SIGTERM, handleStopSignal);

    try {
        spdlog::debug("Setting up application...");
        Application app;
        spdlog::info("Running application...");
        Application::start();
    } catch (const std::exception& error) {
        spdlog::error("Error occurred: {}", error.what());
        return 1;
    } catch (...) {
        spdlog::error("Unknown fatal error occurred");
        return 1;
    }

    return 0;
}
