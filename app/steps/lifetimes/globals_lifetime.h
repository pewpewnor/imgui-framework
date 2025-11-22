#pragma once

#include "engine/shutdown_step.h"
#include "engine/startup_step.h"

class GlobalsLifetime : public engine::StartupStep, public engine::ShutdownStep {
public:
    void onStartup() override;

    void onShutdown() override;
};
