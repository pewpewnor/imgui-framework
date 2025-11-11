#pragma once

#include "shutdown_layer.h"
namespace surface {

class ShutdownGlfwImGui : public engine::ShutdownLayer {
public:
    void execute(const std::shared_ptr<engine::Rigging>& rigging) override;
};

}
