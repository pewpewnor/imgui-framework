#pragma once

#include "engine/shutdown_step.h"
#include "engine/startup_step.h"
#include "globals/app_state.h"
#include "globals/tasks.h"

class GlobalsLifetime : public engine::StartupStep, public engine::ShutdownStep {
public:
    void onStartup() override {
        globals::appState = std::make_unique<globals::AppState>();
        globals::ignoredFutures = std::make_unique<globals::IgnoredFutures>();
        globals::tasks = std::make_unique<globals::Tasks>();
        spdlog::debug("Global variables initialized");
    }

    void onShutdown() override {
        spdlog::debug("Destroying global variables...");
        globals::ignoredFutures.reset();
        globals::tasks.reset();
        globals::appState.reset();
        spdlog::debug("Global variables destroyed");
    }
};
