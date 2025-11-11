#pragma once

#include "startup_step.h"

namespace surface {

class StartupGlfwImGui : public engine::StartupStep {
public:
    StartupGlfwImGui(const std::string& title, int width, int height,
                     bool vsync);

    void onStartup(const std::shared_ptr<engine::Rigging>& rigging) override;

private:
    std::string title_;
    int width_ = 1280;
    int height_ = 720;
    bool vsync_ = true;
    float scale_ = 1;

    void initializeGlfw(const std::shared_ptr<engine::Rigging>& rigging);

    void initializeImGui(const std::shared_ptr<engine::Rigging>& rigging) const;
};

}
