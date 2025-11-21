#pragma once

namespace engine {

class StartupStep {
public:
    StartupStep() = default;
    StartupStep(const StartupStep&) = default;
    StartupStep(StartupStep&&) = delete;
    StartupStep& operator=(const StartupStep&) = default;
    StartupStep& operator=(StartupStep&&) = delete;
    virtual ~StartupStep() = default;

    virtual void onStartup() = 0;
};

}
