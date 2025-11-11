#pragma once

#include "shutdown_stage.h"

namespace surface {

class ShutdownGlfwImGui : public engine::ShutdownStage {
public:
    void onShutdown(const std::shared_ptr<engine::Rigging>& rigging) override;
};

}
