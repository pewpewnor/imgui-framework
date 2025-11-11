#pragma once

#include "shutdown_step.h"

namespace surface {

class ShutdownGlfwImGui : public engine::ShutdownStep {
public:
    void onShutdown(const std::shared_ptr<engine::Rigging>& rigging) override;
};

}
