#pragma once

namespace engine {

class ShutdownStep {
public:
    ShutdownStep() = default;

    virtual void onShutdown() = 0;

    ShutdownStep(const ShutdownStep&) = default;
    ShutdownStep(ShutdownStep&&) = delete;
    ShutdownStep& operator=(const ShutdownStep&) = default;
    ShutdownStep& operator=(ShutdownStep&&) = delete;

    virtual ~ShutdownStep() = default;
};

}
