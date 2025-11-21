#pragma once

namespace engine {

class ShutdownStep {
public:
    ShutdownStep() = default;
    ShutdownStep(const ShutdownStep&) = default;
    ShutdownStep(ShutdownStep&&) = delete;
    ShutdownStep& operator=(const ShutdownStep&) = default;
    ShutdownStep& operator=(ShutdownStep&&) = delete;
    virtual ~ShutdownStep() = default;

    virtual void onShutdown() = 0;
};

}
