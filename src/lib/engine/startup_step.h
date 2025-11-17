#pragma once

namespace engine {

class StartupStep {
public:
    StartupStep() = default;

    virtual void onStartup() = 0;

    StartupStep(const StartupStep&) = default;
    StartupStep(StartupStep&&) = delete;
    StartupStep& operator=(const StartupStep&) = default;
    StartupStep& operator=(StartupStep&&) = delete;

    virtual ~StartupStep() = default;
};

}
